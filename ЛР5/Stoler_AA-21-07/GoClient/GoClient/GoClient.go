package main

import (
	"bufio"
	"fmt"
	"os"
	"time"
)

func main() {
	var myId int
	mesInit := MessageCall(MR_BROKER, MT_INIT, "") // Отправляем запрос на инициализацию соединения с брокером сообщений
	myId = int(mesInit.Header.To)                  // Получаем идентификатор текущего пользователя из ответа
	go func() {                                    // Горутина для получения сообщений от брокера
		for {
			m := MessageCall(MR_BROKER, MT_GETDATA, "") // Отправляем запрос на получение новых сообщений
			if m.Header.Type == MT_DATA {
				fmt.Println(m.Data)
			} else if m.Header.Type == MT_CLIENTS_LIST {
				fmt.Println("Мой Id: ", myId)
				fmt.Println("Все пользователи: ", m.Data, "50 - all users")
			} else {
				time.Sleep(time.Second * 1)
			}
		}
	}()
	time.Sleep(time.Second)             // Ожидаем 1 секунду перед продолжением выполнения программы
	reader := bufio.NewReader(os.Stdin) // Создаем читатель для ввода данных с клавиатуры
	for {
		var addr int32
		fmt.Println("Чтобы отключиться, введите -1")
		fmt.Println("Введите получателя: ")
		fmt.Scanln(&addr)

		if addr == -1 {
			MessageCall(MR_BROKER, MT_EXIT, "")
			return
		}

		fmt.Println("Введите сообщение: ")
		s, err := reader.ReadString('\n')
		if err != nil {
			continue
		}
		message := fmt.Sprintf("[%d] ", myId) + s // Формируем сообщение, добавляя к нему идентификатор текущего пользователя
		MessageCall(addr, MT_DATA, message)
	}
}
