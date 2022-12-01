#include "extremite.h"

void ext_out(int tunfd, char* port){
    int s,n; /* descripteurs de socket */
    int len,on; /* utilitaires divers */
    struct addrinfo * resol; /* résolution */
    struct addrinfo indic = {AI_PASSIVE, /* Toute interface */
                           PF_INET,SOCK_STREAM,0, /* IP mode connecté */
                           0,NULL,NULL,NULL};
    struct sockaddr_in client; /* adresse de socket du client */
    int err; /* code d'erreur */

    err = getaddrinfo(NULL,port,&indic,&resol);
    if (err<0){
        fprintf(stderr,"Résolution: %s\n",gai_strerror(err));
        exit(2);
    }

    /* Création de la socket, de type TCP / IP */
    if ((s=socket(resol->ai_family,resol->ai_socktype,resol->ai_protocol))<0) {
        perror("allocation de socket");
        exit(3);
    }
    fprintf(stderr,"le n° de la socket est : %i\n",s);

    /* On rend le port réutilisable rapidement /!\ */
    on = 1;
    if (setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0) {
        perror("option socket");
        exit(4);
    }
    fprintf(stderr,"Option(s) OK!\n");

    /* Association de la socket s à l'adresse obtenue par résolution */
    if (bind(s,resol->ai_addr,sizeof(struct sockaddr_in))<0) {
        perror("bind");
        exit(5);
    }
    freeaddrinfo(resol); /* /!\ Libération mémoire */
    fprintf(stderr,"bind!\n");

    /* la socket est prête à recevoir */
    if (listen(s,SOMAXCONN)<0) {
        perror("listen");
        exit(6);
    }
    fprintf(stderr,"listen!\n");

    while(1) {
        /* attendre et gérer indéfiniment les connexions entrantes */
        len=sizeof(struct sockaddr_in);
        if( (n=accept(s,(struct sockaddr *)&client,(socklen_t*)&len)) < 0 ) {
            perror("accept");
            exit(7);
        }
        /* Nom réseau du client */
        char hotec[NI_MAXHOST];  char portc[NI_MAXSERV];
        err = getnameinfo((struct sockaddr*)&client,len,hotec,NI_MAXHOST,portc,NI_MAXSERV,0);
        if (err < 0 ){
            fprintf(stderr,"résolution client (%i): %s\n",n,gai_strerror(err));
        }else{
            fprintf(stderr,"accept! (%i) ip=%s port=%s\n",n,hotec,portc);
        }

        //copy(n, tunfd);
        bidirectional_copy(n, tunfd);
    }
}

void ext_in(int tunfd, const char* hote, char* port){
    char ip[NI_MAXHOST]; /* adresse IPv4 en notation pointée */
    struct addrinfo *resol; /* struct pour la résolution de nom */
    int s; /* descripteur de socket */

    while( 1 ) {
        /* Résolution de l'hôte */
        if ( getaddrinfo(hote,port,NULL, &resol) < 0 ){
            perror("résolution adresse");
            exit(2);
        }

        /* On extrait l'addresse IP */
        sprintf(ip,"%s",inet_ntoa(((struct sockaddr_in*)resol->ai_addr)->sin_addr));

        /* Création de la socket, de type TCP / IP */
        /* On ne considère que la première adresse renvoyée par getaddrinfo */
        if ((s=socket(resol->ai_family,resol->ai_socktype, resol->ai_protocol))<0) {
            perror("allocation de socket");
            exit(3);
        }
        fprintf(stderr,"le n° de la socket est : %i\n",s);

        /* Connexion */
        fprintf(stderr,"Essai de connexion à %s (%s) sur le port %s\n\n",
          hote,ip,port);
        if (connect(s,resol->ai_addr,sizeof(struct sockaddr_in))<0) {
            perror("connexion");
            exit(4);
        }
        //copy(tunfd, s);
        bidirectional_copy(tunfd, s);
    }
    /* Destruction de la socket */
    close(s);

    fprintf(stderr,"Fin de la session.\n");
}
