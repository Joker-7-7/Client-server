#include<iostream>
#include<string.h>
#include <memory>

#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных
#define PORT 7777 // Будем использовать этот номер порта

struct Message
{
    std::string word;
    std::string name;
};

struct sockaddr_in serveraddress, client;
socklen_t length;
int sockert_file_descriptor, connection, bind_status, connection_status;
//char message[MESSAGE_LENGTH];

int main() {

    std::unique_ptr<Message> message (new Message);

    // Создадим сокет
    sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sockert_file_descriptor == -1) {
        std::cout << "Socket creation failed.!" << std::endl;
        exit(1);
    }
    // 
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    // Зададим номер порта для связи
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    // Привяжем сокет
    bind_status = bind(sockert_file_descriptor, (struct sockaddr*)&serveraddress,
        sizeof(serveraddress));
    if (bind_status == -1) {
        std::cout << "Socket binding failed.!" << std::endl;
        exit(1);
    }
    // Поставим сервер на прием данных 
    connection_status = listen(sockert_file_descriptor, 5);
    if (connection_status == -1) {
        std::cout << "Socket is unable to listen for new connections.!" << std::endl;
        exit(1);
    }
    else {
        std::cout << "Server is listening for new connection: " << std::endl;
    }
    length = sizeof(client);
    connection = accept(sockert_file_descriptor, (struct sockaddr*)&client, &length);
    if (connection == -1) {
        std::cout << "Server is unable to accept the data from client.!" << std::endl;
        exit(1);
    }
    // Communication Establishment
    while (1) {
        bzero(message.get(), sizeof(Message));
        read(connection, message.get(), sizeof(Message));
        if (message->word == "end") {
            std::cout << "Client Exited." << std::endl;
            std::cout << "Server is Exiting..!" << std::endl;
            break;
        }
        std::cout << "Data received from client: " << message->word << " " << message->name << std::endl;
        bzero(message.get(), sizeof(Message));
        std::cout << "Enter the message you want to send to the client: " << std::endl;
        std::cin >> message->word;
        std::cin >> message->name;
        ssize_t bytes = write(connection, message.get(), sizeof(Message));
        // Если передали >= 0  байт, значит пересылка прошла успешно
        if (bytes >= 0) {
            std::cout << "Data successfully sent to the client.!" << std::endl;
        }
    }

    // закрываем сокет, завершаем соединение
    close(sockert_file_descriptor);
    return 0;
}
