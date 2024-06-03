using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace LR5_Stoler
{
    public enum MessageTypes : int    // Определение типов сообщений
    {
        MT_INIT,
        MT_EXIT,
        MT_GETDATA,
        MT_DATA,
        MT_NODATA,
        MT_CONFIRM,
        MT_CLIENTS_LIST
    };

    public enum MessageRecipients : int    // Определение типов получателей сообщений
    {
        MR_BROKER = 10,
        MR_ALL = 50,
        MR_USER = 100
    };

    // Структура заголовка сообщения с указанием его формата в памяти
    [StructLayout(LayoutKind.Sequential)]
    struct MessageHeader
    {
        //  MarshalAs: Атрибут, указывающий, как поле или параметр должно быть маршализовано 
        //  (преобразовано в формат, пригодный для передачи) при передаче между управляемым и неуправляемым кодом (обычный машинный код).
        //  UnmanagedType.I4: Указывает, что данные должны быть маршализованы как 4-байтовое целое число (32-битное целое).
        
        [MarshalAs(UnmanagedType.I4)]                                                                                                     
        public MessageRecipients to;    // Кому отправлено сообщение
        [MarshalAs(UnmanagedType.I4)]
        public MessageRecipients from;  // От кого отправлено сообщение
        [MarshalAs(UnmanagedType.I4)]
        public MessageTypes type;       // Тип сообщения
        [MarshalAs(UnmanagedType.I4)]
        public int size;                // Размер данных сообщения
    };

    class Message
    {
        public MessageHeader header; // Заголовок сообщения
        public string data;          // Данные сообщения
        static MessageRecipients clientID; // Идентификатор клиента
        static Encoding cp866 = null; // Кодировка CP866

        Encoding get866()   // Метод для получения кодировки CP866
        {
            if (cp866 is null)
            {
                Encoding.RegisterProvider(CodePagesEncodingProvider.Instance); // регистрация поставщика кодировок
                cp866 = Encoding.GetEncoding("CP866"); // получение кодировки CP866
            }
            return cp866;
        }

        // Коснтруктор сообщения
        public Message(MessageRecipients to, MessageRecipients from, MessageTypes type = MessageTypes.MT_DATA, string data = "")
        {
            this.data = data;
            header = new MessageHeader() { to=to, from=from, type=type, size=data.Length };
        }

        // Метод для преобразования объекта в массив байтов
        static byte[] toBytes(object obj)
        {
            int size = Marshal.SizeOf(obj);         // Определение размера объекта в байтах
            byte[] buff = new byte[size];           // Создание буфера байтов нужного размера
            IntPtr ptr = Marshal.AllocHGlobal(size);// Выделение неуправляемой памяти для объекта
            Marshal.StructureToPtr(obj, ptr, true); // Копирование данных объекта в неуправляемую память
            Marshal.Copy(ptr, buff, 0, size);       // Копирование данных из неуправляемой памяти в массив байтов
            Marshal.FreeHGlobal(ptr);               // Освобождение неуправляемой памяти
            return buff;
        }

        // Метод для преобразования массива байтов в объект
        static T fromBytes<T>(byte[] buff) where T : struct
        {
            T data = default(T);       // Создание переменной для хранения десериализованных данных
            int size = Marshal.SizeOf(data); // Определение размера структуры в байтах
            IntPtr i = Marshal.AllocHGlobal(size); // Выделение неуправляемой памяти размером, равным размеру структуры
            Marshal.Copy(buff, 0, i, size); // Копирование данных из массива байтов в неуправляемую память
            var d = Marshal.PtrToStructure(i, data.GetType()); // Преобразование данных из неуправляемой памяти в объект типа T
            if (d != null) // Если преобразование прошло успешно, присваиваем значение переменной data
            {
                data = (T)d;
            }
            Marshal.FreeHGlobal(i); // Освобождение неуправляемой памяти
            return data;
        }

        // Метод получения сообщения через сокет
        MessageTypes receive(Socket s)
        {
            byte[] buff = new byte[Marshal.SizeOf(header)];
            if (s.Receive(buff, Marshal.SizeOf(header), SocketFlags.None) == 0)
            {
                return MessageTypes.MT_NODATA;
            }
            header = fromBytes<MessageHeader>(buff);
            if (header.size > 0)
            {
                byte[] b = new byte[header.size];
                s.Receive(b, header.size, SocketFlags.None);
                data = get866().GetString(b, 0, header.size);

            }
            return header.type;
        }

        // Метод отправки сообщения через сокет
        void send(Socket s)
        {
            s.Send(toBytes(header), Marshal.SizeOf(header), SocketFlags.None); // отправка заголовка
            if (header.size != 0) // если размер данных не 0, то отправка сообщения
            {
                s.Send(get866().GetBytes(data), header.size, SocketFlags.None);
            }
        } // данные отправляются после заголовка, чтобы получатель мог правильно их интерпретировать


        // Статический метод отправки сообщения через сокет
        public static void send(Socket s, MessageRecipients to, MessageRecipients from, MessageTypes type = MessageTypes.MT_DATA, string data = "")
        {
            new Message(to, from, type, data).send(s);

        }

        // Статический метод отправки сообщения с возвратом ответа
        public static Message send(MessageRecipients to, MessageTypes type = MessageTypes.MT_DATA, string data = "")
        {
            int nPort = 22002;
            IPEndPoint endPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), nPort);
            Socket s = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            s.Connect(endPoint);
            if (!s.Connected)
            {
                throw new Exception("Connection error");
            }
            var m = new Message(to, clientID, type, data);
            m.send(s);
            if (m.receive(s) == MessageTypes.MT_INIT)
            {
                clientID = m.header.to;
            }
            return m;
        }
    }
}

//Экземплярный метод send(Socket s): отправляет текущее сообщение через предоставленный сокет.
//Требует предварительного создания и инициализации объекта Message.

//Статический метод send(Socket s, ...): создает и отправляет новое сообщение через предоставленный сокет. 
//Не требует предварительного создания объекта Message.

//Статический метод send(...): создает новое сообщение и отправляет его на фиксированный адрес и порт,  
//создает и управляет сокетом внутри метода.
//Удобен для отправки сообщений на заранее определенный сервер и получения ответа.