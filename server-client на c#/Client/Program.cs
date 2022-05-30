using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace Client
{
    class Program
    {
        static void Main(string[] args)
        {

            int port = 8080;
            string ip = "127.0.0.1";
            IPEndPoint ipPoint = new IPEndPoint(IPAddress.Parse(ip), port);
            Socket soket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            soket.Connect(ipPoint);

            Console.Write("Введите сообщение:");
            string message = Console.ReadLine();
            byte[] buffer = Encoding.UTF8.GetBytes(message);
            soket.Send(buffer);

            int bytes = 0;
            buffer = new byte[256];
            StringBuilder answer = new StringBuilder();
            do
            {
                bytes = soket.Receive(buffer);
                answer.Append(Encoding.UTF8.GetString(buffer, 0, bytes));
            }
            while (soket.Available > 0);
            Console.WriteLine("ответ сервера: " + answer.ToString());
            soket.Shutdown(SocketShutdown.Both);
            soket.Close();
            Console.ReadLine();
        }
    }
}

