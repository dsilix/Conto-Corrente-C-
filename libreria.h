//Sviluppato da Silvio Saturno

#ifndef CONTOCORRENTE_LIBRERIA_H
#define CONTOCORRENTE_LIBRERIA_H
#endif //CONTOCORRENTE_LIBRERIA_H
#define N_UTENTI 10
#define ATTIVO 1                             //STATO DEL CONTO, AL MOMENTO DELLA REGISTRAZIONE DIVENTA "ATTIVO"

typedef struct listaMovimenti{               //CREO UNA STRUCT PER I MOVIMENTI, IN SEGUITO VERRA' CREATO UN ARRAY DI SINGOLI MOVIMENTI
    int anno;
    int mese;
    int giorno;
    float valore;
}listaMovimenti;

typedef struct correntista{                 //QUESTA STRUCT E' PER REGISTRARE I CAMPI UTENTE
    char cognome[25];
    char nome[25];
    int numero_conto;
    float saldo;
    listaMovimenti movimento[100];         //DENTRO DI ESSA CREIAMO UN ARRAY DI MOVIMENTI CON LA STRUCT CREATA IN PRECEDENZA
    int stato;
}correntista;

//PRENDO IN INPUT I DATI DELL'UTENTE E LI REGISTRO NELL'ARRAY utenti CREATO IN banca()
void registra(correntista* utenti, int *contautenti) {
    int counter = 0;
    int id = 0;
    int i;
    if(*contautenti < N_UTENTI){           //CONTROLLA CHE CI SIA SPAZIO PER CREARE NUOVI UTENTI (MAX 10)
        printf("\nInserisci il cognome: ");
        fflush(stdout);
        scanf("%s", &(utenti + *contautenti)->cognome);
        printf("\nInserisci il nome: ");
        fflush(stdout);
        scanf("%s", &(utenti + *contautenti)->nome);
        printf("\nInserisci il deposito iniziale: ");
        fflush(stdout);
        scanf("%f", &(utenti + *contautenti)->saldo);
        while (counter < 3) {              //SERVE SOLO A CREARE UN EFFETTO DI ATTESA MENTRE VIENE ASSEGNATO UN CODICE UNIVOCO
            printf("\nStiamo generando il suo codice id");
            for (int i = 0; i < 4; i++) {
                _sleep(300);
                printf(".");
                counter++;
            }
        }
        id=*contautenti;
        (utenti + *contautenti)->numero_conto = id;  //ASSEGNO ALL'UTENTE UN ID CHE INCREMENTA INSIEME AL NUMERO UTENTI REGISTRATI
        printf("\nIl suo codice id e': %d\n", id);
        (utenti+*contautenti)->stato=ATTIVO;
        ++*contautenti;

    }
    else
        printf("\nNUMERO UTENTI MASSIMO RAGGIUNTO!");

}

//L'UTENTE FA IL LOGIN CON IL SUO CODICE UTENTE, VIENE MOSTRATO IL SALDO. IN SEGUITO
//SELEZIONA IL CODICE DELL'UTENTE A CUI VUOLE MANDARE UN BONIFICO, L'IMPORTO E LA DATA.
//CI SONO TRE CONTROLLI:
//1)AFFINCHE' L'OPERAZIONE VADA A BUON FINE L'UTENTE DEVE ESSERE REGISTRATO (QUINDI ATTIVO).
//2)NON PUO' INVIARE UNA CIFRA SUPERIORE AL SUO SALDO.
//3)LE DATE DEVONO ESSERE "VERITIERE" E IN UN RANGE SPECIFICATO DAL PROGRAMMA IN CASO DI ERRATO INSERIMENTO
//IL MOVIMENTO VIENE INOLTRE REGISTRATO ANCHE PER IL DESTINATARIO.
void movimento(correntista *utenti, listaMovimenti *movimenti, int *contamovimenti){
    int id=0,idDestinatario,i;
    float importo;
    printf("\nEffettua il login inserendo il tuo codice di conto:");
    scanf("%d", &id);
    if((utenti+id)->stato==ATTIVO) {
        printf("\nBenvenuto %s %s il tuo saldo e' di %.2f", (utenti + id)->cognome, (utenti + id)->nome,
               (utenti + id)->saldo);
        printf("\nInserisci il numero del conto a cui vuoi effettuare un bonifico: ");
        scanf("%d", &idDestinatario);
        if ((utenti + idDestinatario)->stato == ATTIVO) {

            do {
                printf("\nInserisci l'importo del bonifico:");
                scanf("%f", &importo);
                printf("\nOra ti chiedero' giorno, mese e anno in cui far partire il bonifico:");
                printf("\nInserisci l'anno (AAAA):");
                scanf("%d", &(utenti + id)->movimento[*contamovimenti].anno);
                (utenti + idDestinatario)->movimento[*contamovimenti].anno = (utenti +
                                                                              id)->movimento[*contamovimenti].anno;
                printf("\nInserisci il mese (MM):");
                scanf("%d", &(utenti + id)->movimento[*contamovimenti].mese);
                (utenti + idDestinatario)->movimento[*contamovimenti].mese = (utenti +
                                                                              id)->movimento[*contamovimenti].mese;
                printf("\nInserisci il giorno (GG):");
                scanf("%d", &(utenti + id)->movimento[*contamovimenti].giorno);
                (utenti + idDestinatario)->movimento[*contamovimenti].giorno = (utenti +
                                                                                id)->movimento[*contamovimenti].giorno;
                if (importo > (utenti + id)->saldo) {
                    printf("IMPORTO NON DISPONIBILE SUL PROPRIO CONTO, RIPROVARE");
                    break;
                } else {
                    (utenti + id)->saldo = (utenti + id)->saldo -
                                           importo;                            //L'IMPORTO VIENE SOTTRATTO AL SALDO DEL MITTENTE
                    (utenti + idDestinatario)->saldo = (utenti + idDestinatario)->saldo +
                                                       importo;  //L'IMPORTO VIENE SOMMATO AL SALDO DEL DESTINATARIO
                    (utenti + id)->movimento[*contamovimenti].valore = -importo;
                    (utenti + idDestinatario)->movimento[*contamovimenti].valore = +importo;
                    ++*contamovimenti;        //*contamovimenti VIENE INCREMENTATO, IN MODO DA FAR SCORRERE L'ARRAY IN CUI VENGONO
                    break;                    //REGISTRATE LE TRANSAZIONI
                }
            } while (importo > (utenti + id)->saldo);
        }
        else
        printf("CODICE DESTINATARIO NON TROVATO!");
    }
    else
        printf("CODICE CORRENTISTA NON CORRETTO\n\n");

}

//L'UTENTE SELEZIONA IL CODICE CONTO DEL QUALE VUOLE RICEVERE INFORMAZIONI, IN SEGUITO SELEZIONA
//LA DATA E VISUALIZZERA' TUTTE LE TRANSAZIONI EFFETTUATE IN QUEL GIORNO.
//CIO' E' VISIBILE SIA INSERENDO IL MITTENTE CHE IL DESTINATARIO
void visualizzaMovimenti(correntista *utenti,int *contamovimenti) {
    int i, id, anno, mese, giorno;
    printf("\nInserisci il codice conto del quale vuoi visualizzare i movimenti: ");
    fflush(stdin);
    scanf("%d", &id);
    if ((utenti + id)->stato == ATTIVO) {
        printf("\nInserici l'anno, il mese e il giorno in cui sono stati effettuati i movimenti");
        printf("\nAAAA:");
        scanf("%d", &anno);
        //I TRE CICLI WHILE A SEGUIRE CONTROLLANO SE LE DATE SONO CORRETTE E REGISTRANO IL VALORE SOLO CON DATE VERITIERE
        while (anno < 1900 || anno > 2030) {
            printf("ANNO NON VALIDO! INSERISCI UN ANNO COMPRESO TRA IL 1900 E IL 2030: ");
            scanf("%d", &anno);
        }
        printf("\nMM:");
        scanf("%d", &mese);
        while (mese < 1 || mese > 12) {
            printf("MESE NON VALIDO! INSERISCI UN MESE COMPRESO TRA 1 E 12: ");
            scanf("%d", &mese);
        }
        printf("\nGG:");
        scanf("%d", &giorno);
        while (giorno < 1 || giorno > 31) {
            printf("GIORNO NON VALIDO! INSERISCI UN GIORNO COMPRESO TRA 1 E 31: ");
            scanf("%d", &giorno);
        }
    }
    else
        printf("UTENTE NON TROVATO!");


    if ((utenti + id)->stato == ATTIVO) {
        printf("\n\tLISTA MOVIMENTI:\n");
        for (i = 0; i < *contamovimenti; i++) {
            if ((utenti + id)->movimento[i].anno == anno && (utenti + id)->movimento[i].mese == mese &&
                (utenti + id)->movimento[i].giorno == giorno) {
                if ((utenti + id)->movimento[i].valore > 0) {
                    printf("\tDATA: %d/%d/%d | IMPORTO: +%.2f\n", (utenti + id)->movimento[i].anno,
                           (utenti + id)->movimento[i].mese, (utenti + id)->movimento[i].giorno,
                           (utenti + id)->movimento[i].valore);
                } else {
                    printf("\tDATA: %d/%d/%d | IMPORTO: %.2f\n", (utenti + id)->movimento[i].anno,
                           (utenti + id)->movimento[i].mese, (utenti + id)->movimento[i].giorno,
                           (utenti + id)->movimento[i].valore);
                }
            }
        }
        printf("\n\n");
    }
    else
        printf("\nUTENTE NON TROVATO!");
}

//L'UTENTE PUO SELEZIONARE DAL MENU QUALE OPERAZIONE COMPIERE
void menubanca(){
    int contautenti=0;
    int contamovimenti=0;
    correntista utenti[N_UTENTI];
    listaMovimenti movimenti[100];
    while(1)
    {
        printf("\nMENU' BANCA:\n");
        printf(   "\n1) Registra un nuovo correntista\n"
                  "2) Effettua un bonifico\n"
                  "3) Visualizza lo storico delle transazioni\n"
                  "Q) Esci dalla banca\n");
        printf("\nSeleziona:");

        char c;
        scanf(" %c", &c);
        switch(c){
            case'1':
                registra(utenti,&contautenti);
                break;
            case'2':
                movimento(utenti,movimenti,&contamovimenti);
                break;
            case '3':
                visualizzaMovimenti(utenti,&contamovimenti);
                break;
            case 'Q':
                printf("\nGrazie per aver usato la nostra banca!\n\n\tgithub.com/dsilix\n\n");
                exit(0);
        }
    }
}