#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
// #include <string>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <sys/fcntl.h>

int main(int argc, char* argv[]) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(4001);

    int fd_read = open("./serial_out", O_RDONLY);

    while (1) {
        // int req_size = 30;
        // char buf_ptr[50];
        // int read_size = 0;
        // // memset(buf_ptr, 0, sizeof(buf_ptr));
        // // read_size = read(fd_read, buf_ptr, req_size);
        //
        // if (read_size < 1) {
        //     continue;
        // }
        // buf_ptr[read_size] = '\0';
        // uint8_t buf_ptr[6] = "ojima";
        // // std::string str = std::string(buf_ptr);
        // // printf("%d   %s\n", read_size, buf_ptr);

        int req_size = 100*sizeof(int32_t);
        int32_t buf_ptr[100] = {
            0
        };

        int read_size = read(fd_read, buf_ptr, req_size);
        printf("read %d byte: %08x\n", read_size, buf_ptr[0]);
        // printf("read %d byte: %d, %d\n", read_size, (int8_t)((buf_ptr[0] & 0xff00) >> 8), (int8_t)(buf_ptr[0] & 0x00ff));


        if (read_size == 4) {
            if ((buf_ptr[0] & 0xffff0000) == 0x43000000) {
                // segway_rmp::jyja msg;
                // msg.leftright = 50*(int8_t)((buf_ptr[0] & 0x0000ff00) >> 8) /127.0;
                // msg.frontrear = 1.0*(int8_t)(buf_ptr[0] & 0x000000ff)/127.0;
                // jyja_pub.publish(msg);

                sendto(sockfd, buf_ptr, 1*sizeof(int32_t), 0, (struct sockaddr *)&addr, sizeof(addr));
            }
            else if ((buf_ptr[0] & 0xff000000) == 0xab000000) {
                // segway_rmp::AccelCmd msg;
                // msg.T2 = 20*(int8_t)((buf_ptr[0] & 0x00ff0000) >> 12)/127.0;
                // msg.a = 0.5*(int8_t)((buf_ptr[0] & 0x0000ff00) >> 8) /127.0;
                // msg.vel_limit = 1.0*(int8_t)(buf_ptr[0] & 0x000000ff)/127.0;
                // msg.reverse = 1;
                // accel_pub.publish(msg);
            }
            else if ((buf_ptr[0] & 0xff000000) == 0xaf000000) {
                // segway_rmp::AccelCmd msg;
                // msg.T2 = 20*(int8_t)((buf_ptr[0] & 0x00ff0000) >> 12)/127.0;
                // msg.a = 0.5*(int8_t)((buf_ptr[0] & 0x0000ff00) >> 8) /127.0;
                // msg.vel_limit = 1.0*(int8_t)(buf_ptr[0] & 0x000000ff)/127.0;
                // msg.reverse = 0;
                // accel_pub.publish(msg);
            }
            else if (buf_ptr[0] == 0x99999999) {
                // std::cout << "segway_rmp_node を終了\n";
                // std_msgs::String msg;
                // msg.data = "quit";
                // halt_pub.publish(msg);
            }
        }

    }
    close(sockfd);
    return 0;
}
