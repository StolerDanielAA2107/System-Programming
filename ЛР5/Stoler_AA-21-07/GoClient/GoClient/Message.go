package main

import (
	"bytes"
	"encoding/binary"
	"io"
	"net" // функционал для работы с сетевыми соединениями
	"unsafe"

	"golang.org/x/text/encoding/charmap"
	"golang.org/x/text/transform"
)

const (
	MT_INIT = iota
	MT_EXIT
	MT_GETDATA
	MT_DATA
	MT_NODATA
	MT_CONFIRM
	MT_CLIENTS_LIST
)

const (
	MR_BROKER = 10
	MR_ALL    = 50
	MR_USER   = 100
)

type MsgHeader struct {
	To   int32
	From int32
	Type int32
	Size int32
}

func (h MsgHeader) Send(conn net.Conn) {
	buff := new(bytes.Buffer)                  // Создание нового буфера для записи данных
	binary.Write(buff, binary.LittleEndian, h) // Записываем структуру MsgHeader в бинарный формат в буфер с использованием порядка байт little-endian
	conn.Write(buff.Bytes())
}

func (h *MsgHeader) Receive(conn net.Conn) {
	buff := make([]byte, unsafe.Sizeof(*h)) // создание буфера для чтения данных из соединения
	_, err := conn.Read(buff)               // чтение данных из соединения в буфер
	//Если нет ошибок, декодирование данных из буфера в структуру MsgHeader
	if err == nil {
		binary.Read(bytes.NewBuffer(buff), binary.LittleEndian, h)
	} else {
		h.Size = 0
		h.Type = MT_NODATA
	}
}

func from866(b []byte) string {
	reader := transform.NewReader(bytes.NewReader(b), charmap.CodePage866.NewDecoder())
	r, _ := io.ReadAll(reader)
	return string(r)
}

func to866(s string) []byte {
	buff := new(bytes.Buffer)                                             // Создаем новый буфер для записи закодированных данных
	writer := transform.NewWriter(buff, charmap.CodePage866.NewEncoder()) // Создаем писатель для кодирования данных из Unicode в CP866
	defer writer.Close()                                                  // Закрываем писатель по завершении выполнения функции
	writer.Write([]byte(s))                                               // Записываем данные строки s в писатель, чтобы они были закодированы в CP866
	return buff.Bytes()                                                   // Возвращаем закодированные данные из буфера
}

type String string

func (s String) to866() []byte {
	buff := new(bytes.Buffer)
	writer := transform.NewWriter(buff, charmap.CodePage866.NewEncoder())
	defer writer.Close()
	writer.Write([]byte(string(s)))
	return buff.Bytes()
}

type Message struct {
	Header MsgHeader
	Data   string
}

func (m *Message) init(To int32, From int32, Type int32, Data string) {
	m.Header = MsgHeader{To, From, Type, int32(len(Data))}
	m.Data = Data
}

func (m Message) Send(conn net.Conn) {
	m.Header.Send(conn)
	if m.Header.Size > 0 {
		conn.Write(to866(m.Data))
	}
}

func (m *Message) Receive(conn net.Conn) int32 {
	m.Header.Receive(conn)
	if m.Header.Size > 0 {
		buff := make([]byte, m.Header.Size)
		conn.Read(buff)
		m.Data = from866(buff)
	}
	return m.Header.Type
}

var clientID int32 = 0

func MessageSend(conn net.Conn, To int32, From int32, Type int32, Data string) *Message {
	m := new(Message)
	m.init(To, From, Type, Data)
	m.Send(conn)
	return m // возврат указателя на сообщение
}

func MessageCall(To int32, Type int32, Data string) *Message {
	conn, _ := net.Dial("tcp", "localhost:22002")    // Соединение с сервером// _ игнорирование возвращаемого значения функции
	defer conn.Close()                               // Закрываем соединение по завершении выполнения функции
	m := MessageSend(conn, To, clientID, Type, Data) // Отправляем сообщение и получаем ответ
	m.Receive(conn)
	if m.Header.Type == MT_INIT {
		clientID = m.Header.To
	}
	return m
}
