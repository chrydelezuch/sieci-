#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <errno.h>

#include "Database.h"
#include "UserMenager.h"


#define MAX_EVENTS 32
#define PORT 1234
#define BUF_SIZE 1024

// Ustawienie deskryptora w tryb nieblokujący
void setnonblock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) { perror("fcntl F_GETFL"); exit(EXIT_FAILURE); }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL"); exit(EXIT_FAILURE);
    }
}

int main() {
    
    database::Database db("example.db");
    UserManager& userMenager = UserManager::getInstance();

    int sfd, cfd, efd, nfds, i;
    struct sockaddr_in saddr, caddr;
    struct epoll_event event, events[MAX_EVENTS];
    socklen_t slt = sizeof(caddr);
    char buf[BUF_SIZE];

    // Tworzymy gniazdo serwera
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) { perror("socket"); exit(EXIT_FAILURE); }
    setnonblock(sfd);

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(PORT);

    if (bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr)) == -1) {
        perror("bind"); exit(EXIT_FAILURE);
    }

    if (listen(sfd, 10) == -1) {
        perror("listen"); exit(EXIT_FAILURE);
    }

    // Tworzymy epoll
    efd = epoll_create1(0);
    if (efd == -1) { perror("epoll_create1"); exit(EXIT_FAILURE); }

    event.events = EPOLLIN;  // monitorujemy nowe połączenia
    event.data.fd = sfd;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event) == -1) {
        perror("epoll_ctl"); exit(EXIT_FAILURE);
    }

    printf("Serwer uruchomiony na porcie %d\n", PORT);

    // Główna pętla serwera
    while (1) {
        nfds = epoll_wait(efd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            if (errno == EINTR) continue;  // sygnały
            perror("epoll_wait"); exit(EXIT_FAILURE);
        }

        for (i = 0; i < nfds; i++) {
            if (events[i].data.fd == sfd) {
                // Nowe połączenie
                while ((cfd = accept(sfd, (struct sockaddr*)&caddr, &slt)) != -1) {
                    setnonblock(cfd);
                    event.events = EPOLLIN;  // monitorujemy odczyt danych od klienta
                    event.data.fd = cfd;
                    if (epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &event) == -1) {
                        perror("epoll_ctl add client"); close(cfd);
                    }
                    printf("Nowe połączenie, fd=%d\n", cfd);
                }
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    perror("accept");
                }
            } else {
                // Dane od klienta
                int n = read(events[i].data.fd, buf, BUF_SIZE);
                if (n > 0) {
                    buf[n] = '\0';

                    //printf("Odebrano od fd=%d: %s", events[i].data.fd, buf);
                    //write(events[i].data.fd, buf, n);
                    /**TO DO
                     ///
                        *
                        * prasowanie danych z json
                        * 
                    ///
                     #### rejestracja nowych użytkowników: 
                

                     db.addUser(username, password);

                     #### logowanie:

                     Database::User user = Database::getUserByUsername(username);

                    ///
                        *
                        * sprawdzanie hasła oraz urzytkownika
                        * 
                    ///

                     userMenager.add(User::User(generateSessionId(dlugoscId), username, db.getUserChannels(user.id), events[i].data.fd));

                    ### dodawanie kanału:

                     db.addChannel(channelName);

                    ### dodawanie urzytkownika do kanału:

                     db.addUserToChannel(int userId, int channelId);

                     userMenager.updateUserChannels(username);
                    
                    ### wsyłanie wiadomości do wszystkich członków kanału:

                    std::vector<User*> UserManager users = userMenager.getUsersByChannel(channel);
                    for (auto& u : users) {
                        write(u.getSocketDescriptor(), buf, n);
                    }

                    ### wylogowanie użytkownika

                    userMenager.removeUser(username);


                     


                    */

                } else if (n == 0 || (n == -1 && errno != EAGAIN)) {
                    // Klient zamknął połączenie
                    printf("Zamykam fd=%d\n", events[i].data.fd);
                    epoll_ctl(efd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    close(events[i].data.fd);
                }
            }
        }
    }

    close(sfd);
    close(efd);
    return 0;
}
