#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h> 
#include <pthread.h>
#include <queue>
#include <mutex>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <set>
#include "html_parser.h"



//Thread Queue Lock
pthread_mutex_t QueueLock;
std::queue <int> event_queue; // Events are Socket Numbers


//Can be upgraded by letting multiple reading using semaphore
pthread_mutex_t DictionaryLock; // Mutex For Reading-Writing of dictionary.txt


class website_handler
{
private:
    std::set <std::string> dictionary; // Current dictionary set
    
    std::map<std::string,char*> page; // "page name" -> page_contents
    
    char *readFile(const char *fileName) // Read File and return the output as a char pointer
    {
        FILE *file = fopen(fileName, "r");
        char *code;
        size_t n = 0;
        int c;
        if (file == NULL)
            return NULL;
        code = (char *) malloc(50000 * sizeof(char));
        while ((c = fgetc(file)) != EOF)
            code[n++] = (char) c;
        code[n] = '\0';
        return code;
    }
public:
    website_handler()
    {
//        init_dictionary();
    }
    void load(const char *filename) // Loading html file to proccess
    {
        page[filename]=readFile(filename);
    }

     /**
      * @brief  Returns the wanted page for given params
      * @param  *filename: 
      * @param  request_type: get => 0 , post => 1 , put=> 2
      * @param  input: Add Get request input
      * @param  text: Add  Body of request(for POST Request)
      * @retval Char Pointer 
      */
    char* get_page(const char *filename,int request_type,std::string input,std::string text)
    {
        std::string str = "HTTP/1.1 200 Okay\r\nContent-Type: text/html; charset=ISO-8859-4 \r\n\r\n" + std::string(page[filename]);
        if(request_type==1)
        {
            add_dictionary(text);
        }
        else if(request_type==0 && input!="")
        {
            int is_contains = check_dictionary(input);
            if(is_contains)
            {
                input+=" is found in your Dictionary";
                str.replace( str.find("<!Rvalue>") , 9, input.c_str()) ;
            }
            else
            {
                input+=" is NOT found in your Dictionary";
                str.replace( str.find("<!Rvalue>") , 9, input.c_str()) ;
            }
        }
        char *cstr = new char[str.length() + 1];
        strcpy(cstr, str.c_str());
        return cstr;
    }

    /**
     * @brief  Add Word To Dictionary (Both Set and File)
     * @note   
     * @param  word: Word To be added, c++ stl format
     */
    void add_dictionary(std::string word) // 
    {
        pthread_mutex_lock(&DictionaryLock);
        if (dictionary.count(word)==0)
        {
            FILE *fp;
            fp = fopen("dictionary.txt", "a+");
            fprintf(fp, "%s\n",word.c_str());
            dictionary.insert(word);
            fclose(fp);
        }
        pthread_mutex_unlock(&DictionaryLock);
    }
    /**
     * @brief  Add Word To Dictionary (Both Set and File)
     * @note   
     * @param  word: Word To be added, c++ stl format
     */
     int check_dictionary(std::string word)
    {
        pthread_mutex_lock(&DictionaryLock);
        int return_value = dictionary.count(word);
        pthread_mutex_unlock(&DictionaryLock);
        return return_value;
    }
    /**
     * @brief  Read dictionary.txt and add all to dictionary set for fast access
     */
    void init_dictionary()
    {
        pthread_mutex_lock(&DictionaryLock);
        dictionary.insert("");
        FILE *fp;
        fp = fopen("dictionary.txt", "r");
        if(fp == NULL || fp==0) //if file does not exist, create it
        {
            fp = fopen("dictionary.txt", "w");
            fclose(fp);
            pthread_mutex_unlock(&DictionaryLock);
            return;
        }
        std::string word;
        char c;
        while((c = getc(fp)) != EOF) {
            if(c=='\n')
            {
                dictionary.insert(word);
                word.clear();
            }
            else word.push_back(c);
        }
        if(word!="")
            dictionary.insert(word);
        fclose(fp);
        pthread_mutex_unlock(&DictionaryLock);
    }
};

website_handler website;



class server
{
private:

    int file_descriptor;
    int sizeof_address;
    int THREAD_COUNT; 


    struct sockaddr_in address;
    int server_up;
    int new_socket()  // New socket for listen
    {
        file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
        if (file_descriptor <= 0)
        {
            perror("ERROR: Could'nt open socket");
            return -1;
        }
        return 0;
    }
    int bind_address() // Bind address to socket
    {
        int return_value=bind(file_descriptor, (struct sockaddr *) &address, sizeof(address));
        if (return_value < 0)
        {
            perror("ERROR: Could'nt bind");
            return -1;
        }
        return 0;
    }
    int start_listen(int k=100000) // k is the max size of the queue 
    {
        int return_value = listen(file_descriptor, k);
        if (return_value < 0)
        {
            perror("ERROR: Could'nt listen");
            return -1;
        }
        return 0;
    }
     /**
      * @brief  accepts new requests from file_descriptor,
      * @retval int, the value of the connection socket 
      */
    int accept_connection() 
    {
        int connection_value = accept(file_descriptor, (struct sockaddr *)&address, (socklen_t*)&sizeof_address);
        if (connection_value < 0)
        {
            perror("ERROR: Connection Accept Failure");
            return -1;
        }
        return connection_value;
    }
public:
   
     /**
      * @brief  Server Constructer
      * @param  internet_address: internet address
      * @param  port_number: port number, Default:80
      * @param  THREAD_COUNT: Number Of Thread Count for a proccess
      * @retval 
     */
    server(int internet_address,int port_number=80,int THREAD_COUNT=10) // 80 for http
    {
        this->THREAD_COUNT = THREAD_COUNT;
        server_up=0;
        sizeof_address=sizeof(address);
        address.sin_family = AF_INET; // Internet Based
        if(internet_address==0)
            address.sin_addr.s_addr = INADDR_ANY;  // accept any incoming
        else
            address.sin_addr.s_addr = internet_address; 
        address.sin_port =  htons(  80);   // htons(port_number)     
        memset(address.sin_zero, '\0', sizeof address.sin_zero);
        if(new_socket() == -1)
            return ;
        if(bind_address() == -1)
            return ;
        if(start_listen() == -1)
            return ;
        server_up=1;
    }
    ~server()
    {
        shutdown(file_descriptor,2);
    }
    /**
     * 
     */
    static void* connection_thread(void *argv)
    {
        while(true)
        {
            int socket_num;
            pthread_mutex_lock(&QueueLock);
            if(event_queue.empty()==false)
            {
                socket_num = event_queue.front();
                event_queue.pop();
                pthread_mutex_unlock(&QueueLock);
            }
            else
            {
                pthread_mutex_unlock(&QueueLock);
                continue;                
            }
            
            char buffer[1000] ={0};
            bzero(buffer,1000);
            int buffer_length=recv(socket_num, buffer,1000, 0);
            if(buffer_length<0)
            {
                perror("ERROR: Receiving Failure");
                return NULL;
            }
            html_parser request(buffer,buffer_length);
            
            
            char *message = website.get_page("main.html",request.get_request_type(),request.get_input("name"),request.get_text());

            int length = strlen(message); 
            int send_value=send(socket_num,message,length,0);
            if(send_value<0)
            {
                perror("ERROR: Sending Failure");
                return NULL;
            }
            close(socket_num);
        }
    }
    void start()
    {
        if(server_up==0) return;
        pthread_t ptid[THREAD_COUNT];
        for (int i = 0; i < THREAD_COUNT; i++)
        {
            int return_value=pthread_create(&ptid[i], NULL, connection_thread, (void*) NULL); 
            if(return_value<0)
            {
                perror("ERROR: Could'nt create thread");
                exit(1);
            }
        }
        while(1) {
            int socket_num=accept_connection();
            pthread_mutex_lock(&QueueLock);
            event_queue.push(socket_num);
            pthread_mutex_unlock(&QueueLock);
        }
    }
};



int main(int argc, char const *argv[])
{
    website.init_dictionary();
    website.load("main.html");
    server basic_server(0,80,10);
    basic_server.start();
    return 0;
}