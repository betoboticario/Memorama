#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <ctime>
#include <iomanip>

#define FLECHA_ARRIBA 72
#define FLECHA_IZQ 75
#define FLECHA_DER 77
#define FLECHA_ABAJO 80
#define ENTER 13

#define MINCOL 1
#define MAXCOL 211
#define MINREN 1
#define MAXREN 50

using namespace std;
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void gotoxy(short x,short y);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

// Modos del cursor
enum modoCursor{MINI = 5, NORMAL = 20, SOLIDO = 80};
//-----------------------------------------------------------------------------------------------------------------------------------------------------
// Estados del cursor
enum estadoCursor{APAGADO, ENCENDIDO};
//-----------------------------------------------------------------------------------------------------------------------------------------------------

enum simbSimple{BV = 179, ESD = 191, EII, BH = 196, EID = 217, ESI}; //Enumeraci�n para el marco.
//-----------------------------------------------------------------------------------------------------------------------------------------------------

enum miColor{
    NEGRO,
    AZUL,
    VERDE,
    AGUAMARINA,
    ROJO,
    PURPURA,
    AMARILLO,
    GRISCLARO,
    GRIS,
    AZULCLARO,
    VERDECLARO,
    AGUAMARINACLARO,
    ROJOCLARO,
    PURPURACLARO,
    AMARILLOCLARO,
    BLANCO
};
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void EstableceColor(int colorFondo, int colorTexto);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

enum EleccionUsuario{JUGAR = 1, VER_INSTRUCCIONES, SALIR};
//-----------------------------------------------------------------------------------------------------------------------------------------------------

enum Dificultad{PRINCIPIANTE = 1, INTERMEDIO, EXPERTO};
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void ImprimeMarco(int ancho, int alto, int x, int y);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void BorraMarco(int ancho, int largo, int x, int y);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void CambiaCursor(estadoCursor estado, modoCursor modo = NORMAL);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

int MenuPrincipal();
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void Gestion_Jugar();
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void ElegirNombresJugadores(char J1[], char J2[]);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

int ElegirDificultad();
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void Tablero(int dificultad, int &marcador1, int &marcador2);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void BarajarCartas(int A[], int n);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void GeneraTablero(int Tablero[][10], int renTablero, int MazoCartas[]);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void Juego_Principiante(int &marcador1, int &marcador2);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void ImprimeTablero(int Tablero[][10], int renTablero);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void ImprimeCartaVolteada(int ancho, int alto, int x, int y);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void Juego_Intermedio(int &marcador1, int &marcador2);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void Juego_Experto(int &marcador1, int &marcador2);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void MoverPosicionTablero_Horizontal(int &x, int y, int dificultad, int direccion);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void MoverPosicionTablero_Vertical(int x, int &y, int dificultad, int direccion);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void ImprimeMarcador(int &marcador, int jugador);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void MostrarCarta(int ancho, int alto, int x, int y);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
    EstableceColor(BLANCO, PURPURA);

    system("pause"); //Pausa para que se ponga en pantalla completa esta madre.
    //Limpi� la pantalla porque quedaba un pedazo del mensaje de la pausa anterior en la esquina superior derecha, ver�s qu�tenle esta
    //pausa y chequenlo.
    system("cls");
    int opcion;

    opcion = MenuPrincipal();

    switch(opcion){
        case 1: Gestion_Jugar(); break;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
void gotoxy(short x,short y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {x-1,y-1};
    SetConsoleCursorPosition(handle,coord);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------

void CambiaCursor(estadoCursor estado, modoCursor modo)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ConCurInf;

    ConCurInf.dwSize = modo;
    ConCurInf.bVisible = estado;

    SetConsoleCursorInfo(hOut, &ConCurInf);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------

void ImprimeMarco(int ancho, int alto, int x, int y)
{
    // Parte superior del marco
    gotoxy(x,y);
    cout << (char)ESI;
    for(int i = 1 ; i <= ancho - 2 ; ++i) cout << (char)BH;
    cout << (char)ESD;

    // Parte media del marco
    for(int i = 1 ; i <= alto - 1 ; ++i){
        gotoxy(x,++y);
        cout << (char)BV;
        for(int j = 1 ; j <= alto - 1 ; ++j){
            gotoxy(x+ancho-1, y);
            cout << (char)BV;
        }
    }

    // Parte inferior del marco
    gotoxy(x,++y);
    cout << (char)EII;
    for(int i = 1 ; i <= ancho - 2 ; ++i) cout << (char)BH;
    cout << (char)EID;

}
//--------------------------------------------------------------------------------------------------------------------------------------------------

void BorraMarco(int ancho, int alto, int x, int y)
{
    gotoxy(x,y);
    for(int i = 1 ; i <= ancho ; ++i) cout << ' ';

     for(int i = 1 ; i <= alto - 1 ; ++i){
        gotoxy(x,++y);
        cout << ' ';
        for(int j = 1 ; j <= alto - 1 ; ++j){
            gotoxy(x+ancho-1, y);
            cout << ' ';
        }
    }

    // Parte inferior del marco
    gotoxy(x,++y);
    for(int i = 1 ; i <= ancho ; ++i) cout << ' ';
}
//--------------------------------------------------------------------------------------------------------------------------------------------------

int MenuPrincipal()
{
    char tecla; //Este va a se como el centinela que anda checando que tecla presiona el usuario.
    int x = 25, y = 1;//Coordenadas en donde se va a comenzar a imprimir el t�tulo "Memorama"

    //Se imprime el t�tulo (MEMORAMA) en la misma posici�n x, solo la y cambiar� porque ps, no puedo darle un endl.
    gotoxy(x,y++);
    cout << " .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.";
    gotoxy(x,y++);
    cout << "| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |";
    gotoxy(x,y++);
    cout << "| | ____    ____ | || |  _________   | || | ____    ____ | || |     ____     | || |  _______     | || |      __      | || | ____    ____ | || |      __      | |";
    gotoxy(x,y++);
    cout << "| ||_   \\  /   _|| || | |_   ___  |  | || ||_   \\  /   _|| || |   .'    `.   | || | |_   __ \\    | || |     /  \\     | || ||_   \\  /   _|| || |     /  \\     | |";
    gotoxy(x,y++);
    cout << "| |  |   \\/   |  | || |   | |_  \\_|  | || |  |   \\/   |  | || |  /  .--.  \\  | || |   | |__) |   | || |    / /\\ \\    | || |  |   \\/   |  | || |    / /\\ \\    | |";
    gotoxy(x,y++);
    cout << "| |  | |\\  /| |  | || |   |  _|  _   | || |  | |\\  /| |  | || |  | |    | |  | || |   |  __ /    | || |   / ____ \\   | || |  | |\\  /| |  | || |   / ____ \\   | |";
    gotoxy(x,y++);
    cout << "| | _| |_\\/_| |_ | || |  _| |___/ |  | || | _| |_\\/_| |_ | || |  \\  `--'  /  | || |  _| |  \\ \\_  | || | _/ /    \\ \\_ | || | _| |_\\/_| |_ | || | _/ /    \\ \\_ | |";
    gotoxy(x,y++);
    cout << "| ||_____||_____|| || | |_________|  | || ||_____||_____|| || |   `.____.'   | || | |____| |___| | || ||____|  |____|| || ||_____||_____|| || ||____|  |____|| |";
    gotoxy(x,y++);
    cout << "| |              | || |              | || |              | || |              | || |              | || |              | || |              | || |              | |";
    gotoxy(x,y++);
    cout << "| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |";
    gotoxy(x,y++);
    cout << " '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'";

    //Vuelvo a inicializar las coordendadas para que el siguiente mensaje (Jugar) quede centrado.
    x = 88;
    y+=5;

    //Repito lo mismo que con el texto anterior.
    gotoxy(x,y++);
    cout << "       __                      ";
    gotoxy(x, y++);
    cout << "      / /_  ______ _____ ______";
    gotoxy(x,y++);
    cout << " __  / / / / / __ `/ __ `/ ___/";
    gotoxy(x,y++);
    cout << "/ /_/ / /_/ / /_/ / /_/ / /    ";
    gotoxy(x, y++);
    cout << "\\____/\\__,_/\\__, /\\__,_/_/     ";
    gotoxy(x, y++);
    cout << "           /____/              ";


    //Vuelvo a inicializar las coordenadas para centrar el mensaje "Instrucciones".
    x = 70;
    y+=5;

    //Se repite el proceso de impresi�n de mensajito.
    gotoxy(x,y++);
    cout << "    ____           __                       _                      ";
    gotoxy(x,y++);
    cout << "   /  _/___  _____/ /________  ____________(_)___  ____  ___  _____";
    gotoxy(x,y++);
    cout << "   / // __ \\/ ___/ __/ ___/ / / / ___/ ___/ / __ \\/ __ \\/ _ \\/ ___/";
    gotoxy(x,y++);
    cout << " _/ // / / (__  ) /_/ /  / /_/ / /__/ /__/ / /_/ / / / /  __(__  ) ";
    gotoxy(x,y++);
    cout << "/___/_/ /_/____/\\__/_/   \\__,_/\\___/\\___/_/\\____/_/ /_/\\___/____/  ";


    //De nuevo y por �ltima vez inicializo las coordenadas para centrar el �ltimo mensaje (Salir).
    x = 93;
    y+=5;

    //Imprimo el mensaje.
    gotoxy(x,y++);
    cout << "   _____       ___     ";
    gotoxy(x,y++);
    cout << "  / ___/____ _/ (_)____";
    gotoxy(x,y++);
    cout << "  \\__ \\/ __ `/ / / ___/";
    gotoxy(x,y++);
    cout << " ___/ / /_/ / / / /    ";
    gotoxy(x,y++);
    cout << "/____/\\__,_/_/_/_/     ";

    //Este peque�o bloquecillo llevo el cursor a la posici�n del mensaje de "Jugar". Se me hizo l�gico iniciar all�
    //e imprimo el marco alrededor
    EstableceColor(BLANCO, ROJOCLARO);
    x = 86;
    y = 16;

    ImprimeMarco(35, 7, x, y );
    //Este goto regresa el cursor a la esquina superior izquierda del mensajito "Jugar", porque desde all� se comienza a
    //imprimir el marco y estas coordenadas son las que voy a usar como referencia para saber sobre cu�l opci�n est� el usuario
    // as� como tambi�n para facilitar el borrado del marco cuando el usuario se vaya hacia otra opci�n y para facilitar la
    //impresi�n del siguiente marquito.
    gotoxy(x,y);

    CambiaCursor(APAGADO); //Aqu� apago el cursor para que no se vea esa madre parpadeando.
    //El ciclito este pues se va a repetir mientras el usuario no le de enter.
    while (tecla != ENTER){
        tecla = getch(); //Esta sentencia es para esperar que el usuario pique algo en el teclado alv. **AHORA SI VIENE LO PERR�N**
        //Estas coordenadas son las coordenadas del mensajito "Jugar", y si el usuario le pica para abajo pasa lo siguiente:
        if(x == 86 && y == 16 && (tecla == FLECHA_ABAJO )){
            BorraMarco(35, 7, x, y); //Se borra el marquito de "Jugar".
            gotoxy(x-=18,y+=10); //Te vas a la esquina superior izquierda del mensaje de "Instrucciones"
            ImprimeMarco(71, 7, x, y); //Desde all�, imprimes el marco alrededor del mensaje (instrucciones)
            gotoxy(x, y);//Y me regreso a la esquinita superior izquierda para facilitar el proceso, ya se los expliqu�
            tecla = getch(); //Y espero a que el usuario teclee otra cosa.
        }

        if(x == 68 && y == 26 && (tecla == FLECHA_ABAJO )){ //Ahora, si el usuario est� sobre "Instrucciones" y le da pa abajo:
            BorraMarco(71, 7, x, y); //Se borra el marco de "Instrucciones".
            gotoxy(x+=23,y+=10); //El cursor se va a la esquina superior izquierda del mensajito de abajo o sea "Salir".
            ImprimeMarco(27, 7, x, y); //Desde la esquinita se imprime el marco alrededor de "Salir".
            gotoxy(x, y); //Me regreso a la esquinita
            tecla = getch(); //Espero a que el usuario pique otra cosa.
        }
        //***HASTA AQU� SE LE DA CHANCE AL USUARIO DE QUE VAYA HACIA ABAJO, SI LLEGA A ESTE PUNTO Y LE SIGUE PICANDO A LA FLECHA PA ABAJO
        //PUES EL PROGRAMA NO VA A HACER NADA PORQUE NO HAY UN if QUE CONSIDERE ESE EVENTO***

        //Entonces, a partir de aqu� hago lo mismo que antes, pero ahora considerando que el usuario le da a la flechita pa arriba.

        //Si est� en el mensaje de "Salir" y pica pa abajo, ps no va a pasar nada alv xD, pero si le da pa arriba:
        if(x == 91 && y == 36 && (tecla == FLECHA_ARRIBA )){
            BorraMarco(27, 7, x, y); //Borro el marquito alrededor de "Salir"
            gotoxy(x-=23, y-=10); //Me voy a la esquina superior izquierda del mensaje de arriba o sea "Instrucciones"
            ImprimeMarco(71, 7, x, y); //Le imprimo su marquito correspondiente.
            gotoxy(x, y); //Me voy a la esquinita superior izquierda de "Instrucciones"
            tecla = getch(); //Espero a que el usuario teclee algo.
        }

        if(x == 68 && y == 26 && (tecla == FLECHA_ARRIBA )){ //Ahora, si el usuario est� en el mensaje "Instrucciones" ps ya se la saben
            BorraMarco(71, 7, x, y); //Borro el marquito alrededor de "Instrucciones"
            gotoxy(x+=18,y-=10); //Me voy al mensaje de arriba, o sea "Jugar"
            ImprimeMarco(35, 7, x, y); //Le imprimo el marco a la palabrita "Jugar"
            gotoxy(x, y); //Me voy a la esquinita superior izquierda de jugar.
            tecla = getch(); //Espero a que el usuario teclee algo
        }
    }
    //Aqu�, se supone que el usuario ya tecle� ENTER eligiendo una opci�n, entonces, limpio la pantalla para quitar el men�
    system("cls");

    //Y aqu� wachen, usando la coordenada en y sabr� qu� opci�n eligi�, y ps imprim� un mensajito nomas para que vieran que si funcion�.
    //Obviamente le voy a seguir moviendo para ir completando mas esta onda, porque por ejemplo las instrucciones deben tener la opcion de
    //devolverse al men� y as�. pero ps es un avance

    //Los quiero.
    if (y == 16)return JUGAR;

    if (y == 26) return VER_INSTRUCCIONES;

    if (y == 36 ) return SALIR;


}
//--------------------------------------------------------------------------------------------------------------------------------------------------

void Gestion_Jugar()
{
    int dificultad;
    int marcador_J1 = 0, marcador_J2 = 0;
    char Jugador_1[1024];
    char Jugador_2[1024];

    ElegirNombresJugadores(Jugador_1, Jugador_2);

    dificultad = ElegirDificultad();

    EstableceColor(BLANCO, NEGRO);
    ImprimeMarco(210, 40, 2,1);

    EstableceColor(BLANCO, ROJOCLARO);
    int x=20, y=42;
    gotoxy(x, y++);
    cout << "    __               _            ___   ";
    gotoxy(x, y++);
    cout << " __|  |_ _ ___ ___ _| |___ ___   |_  |  ";
    gotoxy(x, y++);
    cout << "|  |  | | | . | .'| . | . |  _|   _| |_ ";
    gotoxy(x, y++);
    cout << "|_____|___|_  |__,|___|___|_|    |_____|";
    gotoxy(x, y++);
    cout << "          |___|                         ";

    gotoxy(x+=42, y-=3);
    cout << Jugador_1;

    x-=42;
    y+=3;
    gotoxy(x,y++);
    cout << " _____         _           ";
    gotoxy(x,y++);
    cout << "|  _  |_ _ ___| |_ ___ ___ ";
    gotoxy(x,y++);
    cout << "|   __| | |   |  _| . |_ -|";
    gotoxy(x,y++);
    cout << "|__|  |___|_|_|_| |___|___|";
    cout << "";
    cout << "";

    ImprimeMarcador(marcador_J1, 1);

    EstableceColor(BLANCO, AZULCLARO);
    //Aceptar 40 caracteres.
    x = 130, y = 42;
    gotoxy(x,y++);
    cout << "    __               _            ___ ";
    gotoxy(x,y++);
    cout << " __|  |_ _ ___ ___ _| |___ ___   |_  |";
    gotoxy(x,y++);
    cout << "|  |  | | | . | .'| . | . |  _|  |  _|";
    gotoxy(x,y++);
    cout << "|_____|___|_  |__,|___|___|_|    |___|";
    gotoxy(x,y++);
    cout << "          |___|                       ";

    gotoxy(x+=39, y-=3);
    cout << Jugador_2;

    x-=39;
    y+=3;
    gotoxy(x,y++);
    cout << " _____         _           ";
    gotoxy(x,y++);
    cout << "|  _  |_ _ ___| |_ ___ ___ ";
    gotoxy(x,y++);
    cout << "|   __| | |   |  _| . |_ -|";
    gotoxy(x,y++);
    cout << "|__|  |___|_|_|_| |___|___|";
    cout << "";
    cout << "";

    ImprimeMarcador(marcador_J2, 2);

    Tablero(dificultad, marcador_J1, marcador_J2);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------

void ElegirNombresJugadores(char J1[], char J2[])
{
    EstableceColor(BLANCO, NEGRO);
    int x = 73, y = 1;
    gotoxy(x, y++);
    cout << "   ___ _   _ _____   ___ ______ ___________ _____ _____ ";
    gotoxy(x, y++);
    cout << "  |_  | | | |  __ \\ / _ \\|  _  \\  _  | ___ \\  ___/  ___|";
    gotoxy(x, y++);
    cout << "    | | | | | |  \\// /_\\ \\ | | | | | | |_/ / |__ \\ `--. ";
    gotoxy(x, y++);
    cout << "    | | | | | | __ |  _  | | | | | | |    /|  __| `--. \\";
    gotoxy(x, y++);
    cout << "/\\__/ / |_| | |_\\ \\| | | | |/ /\\ \\_/ / |\\ \\| |___/\\__/ /";
    gotoxy(x, y++);
    cout << "\\____/ \\___/ \\____/\\_| |_/___/  \\___/\\_| \\_\\____/\\____/ ";

    x = 13;
    y+=3;

    gotoxy(13, y++);
    cout << "  _____       _                 _                           _                         _             ";
    cout << "       _                      _           _                       _            ";
    gotoxy(13,y++);
    cout << " |_   _|     | |               | |                         | |                       | |            ";
    cout << "      | |                    | |         (_)                     | |           ";
    gotoxy(13,y++);
    cout << "   | |  _ __ | |_ _ __ ___   __| |_   _ _______ __ _    ___| |  _ __   ___  _ __ ___ | |__  _ __ ___";
    cout << "    __| | ___    ___ __ _  __| | __ _     _ _   _  __ _  __ _  __| | ___  _ __ ";
    gotoxy(13,y++);
    cout << "   | | | '_ \\| __| '__/ _ \\ / _` | | | |_  / __/ _` |  / _ \\ | | '_ \\ / _ \\| '_ ` _ \\| '_ \\| '__/ _ \\";
    cout << "  / _` |/ _ \\  / __/ _` |/ _` |/ _` |   | | | | |/ _` |/ _` |/ _` |/ _ \\| '__|";
    gotoxy(13,y++);
    cout << "  _| |_| | | | |_| | | (_) | (_| | |_| |/ / (_| (_| | |  __/ | | | | | (_) | | | | | | |_) | | |  __/";
    cout << " | (_| |  __/ | (_| (_| | (_| | (_| |   | | |_| | (_| | (_| | (_| | (_) | |   ";
    gotoxy(13,y++);
    cout << " |_____|_| |_|\\__|_|  \\___/ \\__,_|\\__,_/___\\___\\__,_|  \\___|_| |_| |_|\\___/|_| |_| |_|_.__/|_|  \\___|";
    cout << "  \\__,_|\\___|  \\___\\__,_|\\__,_|\\__,_|   | |\\__,_|\\__, |\\__,_|\\__,_|\\___/|_|   ";
    gotoxy(13,y++);
    cout << "                                                                                                     ";
    cout << "                                       _/ |       __/ |                       ";
    gotoxy(13,y++);
    cout << "                                                                                                     ";
    cout << "                                      |__/       |___/                        ";

    x = 1;
    y += 2;

    EstableceColor(BLANCO, ROJOCLARO);
    gotoxy(x, y++);
    cout << "                             (         )   (                           ";
    gotoxy(x, y++);
    cout << "              (        (     )\\ )   ( /(   )\\ )     _ _        )       ";
    gotoxy(x, y++);
    cout << "   (      (   )\\ )     )\\   (()/(   )\\()) (()/(   _| | |_   ( /(       ";
    gotoxy(x, y++);
    cout << "   )\\     )\\ (()/(  ((((_)(  /(_)) ((_)\\   /(_)) |_  .  _|  )\\())      ";
    gotoxy(x, y++);
    cout << "  ((_) _ ((_) /(_))_ )\\ _ )\\(_))_    ((_) (_))   |_     _| ((_)\\    _  ";
    gotoxy(x, y++);
    cout << " _ | || | | |(_)) __|(_)_\\(_)|   \\  / _ \\ | _ \\    |_|_|    / (_)  (_) ";
    gotoxy(x, y++);
    cout << "| || || |_| |  | (_ | / _ \\  | |) || (_) ||   /             | |     _  ";
    gotoxy(x, y++);
    cout << " \\__/  \\___/    \\___|/_/ \\_\\ |___/  \\___/ |_|_\\             |_|    (_) ";

    y-=2;
    do{
        CambiaCursor(ENCENDIDO, SOLIDO);
        gotoxy(73, y);
        cin.getline(J1, 1024);
    }while(J1[0] == '\0');

    EstableceColor(BLANCO, AZULCLARO);
    gotoxy(x, y+=6);
    cout << "                             (         )   (                           ";
    gotoxy(x, y++);
    cout << "              (        (     )\\ )   ( /(   )\\ )     _ _        )       ";
    gotoxy(x, y++);
    cout << "   (      (   )\\ )     )\\   (()/(   )\\()) (()/(   _| | |_   ( /(       ";
    gotoxy(x, y++);
    cout << "   )\\     )\\ (()/(  ((((_)(  /(_)) ((_)\\   /(_)) |_  .  _|  )(_))      ";
    gotoxy(x, y++);
    cout << "  ((_) _ ((_) /(_))_ )\\ _ )\\(_))_    ((_) (_))   |_     _| ((_)     _  ";
    gotoxy(x, y++);
    cout << " _ | || | | |(_)) __|(_)_\\(_)|   \\  / _ \\ | _ \\    |_|_|   |_  )   (_) ";
    gotoxy(x, y++);
    cout << "| || || |_| |  | (_ | / _ \\  | |) || (_) ||   /             / /     _  ";
    gotoxy(x, y++);
    cout << " \\__/  \\___/    \\___|/_/ \\_\\ |___/  \\___/ |_|_\\            /___|   (_) ";

    y-=2;

    do{
        CambiaCursor(ENCENDIDO, SOLIDO);
        gotoxy(73, y);
        cin.getline(J2, 1024);
    }while(J2[0] == '\0');

    CambiaCursor(APAGADO);

    EstableceColor(BLANCO, NEGRO);
    system("cls");
}

//--------------------------------------------------------------------------------------------------------------------------------------------------

void EstableceColor(int colorFondo, int colorTexto)
{
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorFondo*16 + colorTexto);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------

int ElegirDificultad()
{
    int x = 30, y = 1;

    gotoxy(x,y++);
    cout << " _____ _____ _      _____ _____  _____ _____ _____ _   _  _____   _   _ _   _   ___";
    cout << "   ______ ___________ _____ _____ _   _ _    _____ ___ ______ ";
    gotoxy(x,y++);
    cout << "/  ___|  ___| |    |  ___/  __ \\/  __ \\_   _|  _  | \\ | ||  ___| | | | | \\ | | / _ \\";
    cout << "  |  _  \\_   _|  ___|_   _/  __ \\ | | | |  |_   _/ _ \\|  _  \\";
    gotoxy(x,y++);
    cout << "\\ `--.| |__ | |    | |__ | /  \\/| /  \\/ | | | | | |  \\| || |__   | | | |  \\| |/ /_\\ \\";
    cout << " | | | | | | | |_    | | | /  \\/ | | | |    | |/ /_\\ \\ | | |";
    gotoxy(x,y++);
    cout << " `--. \\  __|| |    |  __|| |    | |     | | | | | | . ` ||  __|  | | | | . ` ||  _  |";
    cout << " | | | | | | |  _|   | | | |   | | | | |    | ||  _  | | | |";
    gotoxy(x,y++);
    cout << "/\\__/ / |___| |____| |___| \\__/\\| \\__/\\_| |_\\ \\_/ / |\\  || |___  | |_| | |\\  || | | |";
    cout << " | |/ / _| |_| |    _| |_| \\__/\\ |_| | |____| || | | | |/ / ";
    gotoxy(x,y++);
    cout << "\\____/\\____/\\_____/\\____/ \\____/ \\____/\\___/ \\___/\\_| \\_/\\____/   \\___/\\_| \\_/\\_| |_/ ";
    cout << "|___/  \\___/\\_|    \\___/ \\____/\\___/\\_____/\\_/\\_| |_/___/  ";

    x = 3;
    y+=3;

    EstableceColor(BLANCO, AZULCLARO);
    gotoxy(x,y++);
    cout << "  ____      ____                 _   _      ____              ____                _      _   _     _____  U _____ u ";
    gotoxy(x,y++);
    cout << "U|  _\"\\ uU |  _\"\\ u     ___     | \\ |\"|  U /\"___|    ___    U|  _\"\\ u  ___    U  /\"\\  u | \\ |\"|   |_ \" _| \\| ___\"|/ ";
    gotoxy(x,y++);
    cout << "\\| |_) |/ \\| |_) |/    |_\"_|   <|  \\| |> \\| | u     |_\"_|   \\| |_) |/ |_\"_|    \\/ _ \\/ <|  \\| |>    | |    |  _|\"   ";
    gotoxy(x,y++);
    cout << " |  __/    |  _ <       | |    U| |\\  |u  | |/__     | |     |  __/    | |     / ___ \\ U| |\\  |u   /| |\\   | |___   ";
    gotoxy(x,y++);
    cout << " |_|       |_| \\_\\    U/| |\\u   |_| \\_|    \\____|  U/| |\\u   |_|     U/| |\\u  /_/   \\_\\ |_| \\_|   u |_|U   |_____|  ";
    gotoxy(x,y++);
    cout << " ||>>_     //   \\\\_.-,_|___|_,-.||   \\\\,-._// \\\\.-,_|___|_,-.||>>_.-,_|___|_,-.\\\\    >> ||   \\\\,-._// \\\\_  <<   >>  ";
    gotoxy(x,y++);
    cout << "(__)__)   (__)  (__)\\_)-' '-(_/ (_\")  (_/(__)(__)\\_)-' '-(_/(__)__)\\_)-' '-(_/(__)  (__)(_\")  (_/(__) (__)(__) (__) ";

    x = 60;
    y += 3;

    EstableceColor(BLANCO, PURPURACLARO);
    gotoxy(x,y++);
    cout << " ________  ___   __    _________  ______   ______    ___ __ __   ______   ______    ________  ______      ";
    gotoxy(x,y++);
    cout << "/_______/\\/__/\\ /__/\\ /________/\\/_____/\\ /_____/\\  /__//_//_/\\ /_____/\\ /_____/\\  /_______/\\/_____/\\     ";
    gotoxy(x,y++);
    cout << "\\__.::._\\/\\::\\_\\\\  \\ \\\\__.::.__\\/\\::::_\\/_\\:::_ \\ \\ \\::\\| \\| \\ \\\\::::_\\/_\\:::_ \\ \\ \\__.::._\\/\\:::_ \\ \\    ";
    gotoxy(x,y++);
    cout << "   \\::\\ \\  \\:. `-\\  \\ \\  \\::\\ \\   \\:\\/___/\\\\:(_) ) )_\\:.      \\ \\\\:\\/___/\\\\:\\ \\ \\ \\   \\::\\ \\  \\:\\ \\ \\ \\   ";
    gotoxy(x,y++);
    cout << "   _\\::\\ \\__\\:. _    \\ \\  \\::\\ \\   \\::___\\/_\\: __ `\\ \\\\:.\\-/\\  \\ \\\\::___\\/_\\:\\ \\ \\ \\  _\\::\\ \\__\\:\\ \\ \\ \\  ";
    gotoxy(x,y++);
    cout << "  /__\\::\\__/\\\\. \\`-\\  \\ \\  \\::\\ \\   \\:\\____/\\\\ \\ `\\ \\ \\\\. \\  \\  \\ \\\\:\\____/\\\\:\\/.:| |/__\\::\\__/\\\\:\\_\\ \\ \\ ";
    gotoxy(x,y++);
    cout << "  \\________\\/ \\__\\/ \\__\\/   \\__\\/    \\_____\\/ \\_\\/ \\_\\/ \\__\\/ \\__\\/ \\_____\\/ \\____/_/\\________\\/ \\_____\\/ ";

    x = 140;
    y += 3;

    EstableceColor(BLANCO, ROJOCLARO);
    gotoxy(x,y++);
    cout << "@@@@@@@@  @@@  @@@  @@@@@@@   @@@@@@@@  @@@@@@@   @@@@@@@   @@@@@@   ";
    gotoxy(x,y++);
    cout << "@@!       @@!  !@@  @@!  @@@  @@!       @@!  @@@    @@!    @@!  @@@  ";
    gotoxy(x,y++);
    cout << "!@!       !@!  @!!  !@!  @!@  !@!       !@!  @!@    !@!    !@!  @!@  ";
    gotoxy(x,y++);
    cout << "@!!!:!     !@@!@!   @!@@!@!   @!!!:!    @!@!!@!     @!!    @!@  !@!  ";
    gotoxy(x,y++);
    cout << "!!!!!:      @!!!    !!@!!!    !!!!!:    !!@!@!      !!!    !@!  !!!  ";
    gotoxy(x,y++);
    cout << "!!:        !: :!!   !!:       !!:       !!: :!!     !!:    !!:  !!!  ";
    gotoxy(x,y++);
    cout << ":!:       :!:  !:!  :!:       :!:       :!:  !:!    :!:    :!:  !:!  ";
    gotoxy(x,y++);
    cout << " :: ::::   ::  :::   ::        :: ::::  ::   :::     ::    ::::: ::  ";
    gotoxy(x,y++);
    cout << ": :: ::    :   ::    :        : :: ::    :   : :     :      : :  :   ";

    x = 1;
    y = 9;
    ImprimeMarco(119, 8, x, y);
    gotoxy(x,y);

    char tecla;

    while(tecla !=  ENTER){
        tecla = getch();
        if (x == 1 && y == 9 && (tecla == FLECHA_ABAJO || tecla == FLECHA_DER )){
            BorraMarco(119, 8, x, y);
            gotoxy(x+=57, y+=10);
            ImprimeMarco(109, 8, x, y);
            gotoxy(x, y);
            tecla = getch();
        }

        if (x == 58 && y == 19 && (tecla == FLECHA_ABAJO || tecla == FLECHA_DER )){
            BorraMarco(109, 8, x, y);
            gotoxy(x+=80, y+=10);
            ImprimeMarco(71, 10, x, y);
            gotoxy(x, y);
            tecla = getch();
        }

        if (x == 138 && y == 29 && (tecla == FLECHA_ARRIBA || tecla == FLECHA_IZQ )){
            BorraMarco(71, 10, x, y);
            gotoxy(x-=80, y-=10);
            ImprimeMarco(109, 8, x, y);
            gotoxy(x, y);
            tecla = getch();
        }

        if (x == 58 && y == 19 && (tecla == FLECHA_ARRIBA || tecla == FLECHA_IZQ )){
            BorraMarco(109, 8, x, y);
            gotoxy(x-=57, y-=10);
            ImprimeMarco(119, 8, x, y);
            gotoxy(x, y);
            tecla = getch();
        }
    }

    system("cls");

    if (x == 1) return PRINCIPIANTE;
    if (x == 58) return INTERMEDIO;
    if (x == 138) return EXPERTO;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------

void Tablero(int dificultad, int &marcador1, int &marcador2)
{
    switch (dificultad){
        case PRINCIPIANTE: Juego_Principiante(marcador1, marcador2); break;
        case INTERMEDIO: Juego_Intermedio(marcador1, marcador2); break;
        case EXPERTO: Juego_Experto(marcador1, marcador2); break;
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------

void Juego_Principiante(int &marcador1, int &marcador2)
{
    srand(time(NULL));
    char tecla;

    int Opcion1, Opcion2, contador_selecciones;
    int i_1, i_2, j_1, j_2; //�ndices elegidos.
    int x_aux, y_aux;

    int CartasPrincipiante[10] = {1,1,2,2,3,3,4,4,5,5};
    int TableroPrincipiante[2][10];

    BarajarCartas(CartasPrincipiante, 10);
    GeneraTablero(TableroPrincipiante, 2, CartasPrincipiante);

    int x = 6;
    int y = 3;
    for (int i = 0 ; i <= 20 ; i += 20){
        for (int j = 0 ; j <= 172 ; j += 43){
            ImprimeCartaVolteada(29, 17, x+j, y+i);
        }
        x=6;
    }
    x--;
    y--;

    gotoxy(1, 100);
    ImprimeTablero(TableroPrincipiante, 2);
    gotoxy(x,y);
    EstableceColor(BLANCO, ROJOCLARO);
    ImprimeMarco(31, 18, x, y);

    contador_selecciones = 0;
    Opcion1=0;
    Opcion2 = 0;


    while(tecla !=  ENTER){
        tecla = getch();
        if (x >= 5 && x < 177 && tecla == FLECHA_DER) MoverPosicionTablero_Horizontal(x,y,PRINCIPIANTE,FLECHA_DER);
        if (x > 5 && x <= 177 && tecla == FLECHA_IZQ) MoverPosicionTablero_Horizontal(x,y,PRINCIPIANTE,FLECHA_IZQ);
        if (y == 2 && tecla == FLECHA_ABAJO) MoverPosicionTablero_Vertical(x,y,PRINCIPIANTE,FLECHA_ABAJO);
        if (y == 22 && tecla == FLECHA_ARRIBA) MoverPosicionTablero_Vertical(x,y,PRINCIPIANTE,FLECHA_ARRIBA);
    }

    if (x == 5 && y == 2){
        Opcion1 = TableroPrincipiante[0][0];
        i_1 = 0;
        j_1 = 0;
        x_aux = x;
        y_aux = y;
    }
    if (x == 48 && y == 2){
        Opcion1 = TableroPrincipiante[0][1];
        i_1 = 0;
        j_1 = 1;
    }
    if (x == 91 && y == 2){
        Opcion1 = TableroPrincipiante[0][2];
        i_1 = 0;
        j_1 = 2;
    }
    if (x == 134 && y == 2){
        Opcion1 = TableroPrincipiante[0][3];
        i_1 = 0;
        j_1 = 3;
    }
    if (x == 177 && y == 2){
        Opcion1 = TableroPrincipiante[0][4];
        i_1 = 0;
        j_1 = 4;
    }
    if (x == 5 && y == 22){
        Opcion1 = TableroPrincipiante[1][0];
        i_1 = 1;
        j_1 = 0;
    }
    if (x == 48 && y == 22){
        Opcion1 = TableroPrincipiante[1][1];
        i_1 = 1;
        j_1 = 1;
    }
    if (x == 91 && y == 22){
        Opcion1 = TableroPrincipiante[1][2];
        i_1 = 1;
        j_1 = 2;
    }
    if (x == 134 && y == 22){
        Opcion1 = TableroPrincipiante[1][3];
        i_1 = 1;
        j_1 = 3;
    }
    if (x == 177 && y == 22){
        Opcion1 = TableroPrincipiante[1][4];
        i_1 = 1;
        j_1 = 4;
    }

    x_aux = x;
    y_aux = y;
    if (TableroPrincipiante[i_1][j_1] != 0) MostrarCarta(29, 17, x, y);
    else Beep(100, 250);

    do{
        tecla = '\0';
        while(tecla !=  ENTER){
            tecla = getch();
            if (x >= 5 && x < 177 && tecla == FLECHA_DER) MoverPosicionTablero_Horizontal(x,y,PRINCIPIANTE,FLECHA_DER);
            if (x > 5 && x <= 177 && tecla == FLECHA_IZQ) MoverPosicionTablero_Horizontal(x,y,PRINCIPIANTE,FLECHA_IZQ);
            if (y == 2 && tecla == FLECHA_ABAJO) MoverPosicionTablero_Vertical(x,y,PRINCIPIANTE,FLECHA_ABAJO);
            if (y == 22 && tecla == FLECHA_ARRIBA) MoverPosicionTablero_Vertical(x,y,PRINCIPIANTE,FLECHA_ARRIBA);
        }

        if (x == 5 && y == 2){
            Opcion2 = TableroPrincipiante[0][0];
            i_2 = 0;
            j_2 = 0;
        }
        if (x == 48 && y == 2){
            Opcion2 = TableroPrincipiante[0][1];
            i_2 = 0;
            j_2 = 1;
        }
        if (x == 91 && y == 2){
            Opcion2 = TableroPrincipiante[0][2];
            i_2 = 0;
            j_2 = 2;
        }
        if (x == 134 && y == 2){
            Opcion2 = TableroPrincipiante[0][3];
            i_2 = 0;
            j_2 = 3;
        }
        if (x == 177 && y == 2){
            Opcion2 = TableroPrincipiante[0][4];
            i_2 = 0;
            j_2 = 4;
        }
        if (x == 5 && y == 22){
            Opcion2 = TableroPrincipiante[1][0];
            i_2 = 1;
            j_2 = 0;
        }
        if (x == 48 && y == 22){
            Opcion2 = TableroPrincipiante[1][1];
            i_2 = 1;
            j_2 = 1;
        }
        if (x == 91 && y == 22){
            Opcion2 = TableroPrincipiante[1][2];
            i_2 = 1;
            j_2 = 2;
        }
        if (x == 134 && y == 22){
            Opcion2 = TableroPrincipiante[1][3];
            i_2 = 1;
            j_2 = 3;
        }
        if (x == 177 && y == 22){
            Opcion2 = TableroPrincipiante[1][4];
            i_2 = 1;
            j_2 = 4;
        }
        if(i_1 == i_2 && j_1 == j_2) Beep(100, 250);
        else MostrarCarta(29,17,x,y);
    }while(i_1 == i_2 && j_1 == j_2);

    if (Opcion1 == Opcion2){
        marcador1++;
        ImprimeMarcador(marcador1, 1);
    }
    else{
        Sleep(2000);
        ImprimeCartaVolteada(29,17,++x_aux, ++y_aux);
        ImprimeCartaVolteada(29,17,++x,++y);
    }
    while(true){
        ;
    }

    gotoxy(1, 100);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------

void BarajarCartas(int A[], int n)
{
    int pos;
    double aux;

    for (int i = 0 ; i < (n-1) ; ++i){
        pos  = rand () % ( n - (i + 1) ) + i + 1;
        aux = A[i];
        A[i] = A[pos];
        A[pos] = aux;
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------

void GeneraTablero(int Tablero[][10], int renTablero, int MazoCartas[])
{
    gotoxy(20, 10);
    int indiceCartas = 0;
    for (int i = 0 ; i < renTablero ; ++i){
        for (int j = 0 ; j < 5 ; ++j) Tablero[i][j] = MazoCartas[indiceCartas++];
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------

void ImprimeTablero(int Tablero[][10], int renTablero)
{
    for (int i = 0 ; i < renTablero ; ++i){
        for (int j = 0 ; j < 5 ; ++j) cout << setw(30) << Tablero[i][j];
        cout << endl;
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------

void ImprimeCartaVolteada(int ancho, int alto, int x, int y)
{
    // Parte superior del marco
    EstableceColor(VERDECLARO, ROJOCLARO);
    gotoxy(x,y);
    for(int i = 0 ; i < ancho ; ++i) cout << ' ';

    // Parte media del marco
    for(int i = 1 ; i <= alto - 2 ; ++i){
        gotoxy(x,++y);
        cout << ' ';
        for(int j = 1 ; j < ancho ; ++j) cout << ' ';
    }

    // Parte inferior del marco
    gotoxy(x,++y);
    for(int i = 0 ; i < ancho ; ++i) cout << ' ';

}
//--------------------------------------------------------------------------------------------------------------------------------------------------
void Juego_Intermedio(int &marcador1, int &marcador2)
{
    srand(time(NULL));

    char tecla;

    int CartasIntermedio[] = {1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10};
    int TableroIntermedio[2][10];

    BarajarCartas(CartasIntermedio, 20);
    GeneraTablero(TableroIntermedio, 2, CartasIntermedio);

    int x = 4;
    int y = 4;
    for (int i = 0 ; i <= 19 ; i += 19){
        for (int j = 0 ; j <= 189 ; j += 21){
            ImprimeCartaVolteada(17, 15, x+j, y+i);
        }
        x=4;
    }
    x--;
    y--;

    gotoxy(x,y);
    EstableceColor(BLANCO, ROJOCLARO);
    ImprimeMarco(18, 16, x, y);

    while(tecla !=  ENTER){
        tecla = getch();

        if (x >= 3 && x < 192 && tecla == FLECHA_DER) MoverPosicionTablero_Horizontal(x,y,INTERMEDIO,FLECHA_DER);
        if (x > 3 && x <= 192 && tecla == FLECHA_IZQ) MoverPosicionTablero_Horizontal(x,y,INTERMEDIO,FLECHA_IZQ);
        if (y == 3 && tecla == FLECHA_ABAJO) MoverPosicionTablero_Vertical(x,y,INTERMEDIO,FLECHA_ABAJO);
        if (y == 22 && tecla == FLECHA_ARRIBA) MoverPosicionTablero_Vertical(x,y,INTERMEDIO,FLECHA_ARRIBA);
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void Juego_Experto(int &marcador1, int &marcador2)
{
    srand(time(NULL));

    char tecla;

    int Opcion1, Opcion2, contador_selecciones;

    int CartasExperto[] = {1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13,14,14,15,15};
    int TableroExperto[3][10];

    BarajarCartas(CartasExperto, 30);
    GeneraTablero(TableroExperto, 3, CartasExperto);

    int x = 8;
    int y = 3;
    for (int i = 0 ; i <= 26 ; i += 13){
        for (int j = 0 ; j <= 182 ; j += 20){
            ImprimeCartaVolteada(18, 11, x+j, y+i);
        }
        x=8;
    }

    x--;
    y--;

    gotoxy(x,y);
    EstableceColor(BLANCO, ROJOCLARO);
    ImprimeMarco(19, 12, x, y);

    while(tecla !=  ENTER){
        tecla = getch();
        if (x >= 7 && x < 187 && tecla == FLECHA_DER) MoverPosicionTablero_Horizontal(x,y,EXPERTO,FLECHA_DER);
        if (x > 7 && x <= 187 && tecla == FLECHA_IZQ) MoverPosicionTablero_Horizontal(x,y,EXPERTO,FLECHA_IZQ);
        if (y >= 2 && y < 28 && tecla == FLECHA_ABAJO) MoverPosicionTablero_Vertical(x,y,EXPERTO,FLECHA_ABAJO);
        if (y > 2 && y <= 28 && tecla == FLECHA_ARRIBA) MoverPosicionTablero_Vertical(x,y,EXPERTO,FLECHA_ARRIBA);
    }

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
void MoverPosicionTablero_Horizontal(int &x, int y, int dificultad, int direccion)
{
    int aumentoX, anchoMarco, AltoMarco;

    if (dificultad == PRINCIPIANTE){
        aumentoX = 43;
        anchoMarco = 31;
        AltoMarco = 18;
    }

    if (dificultad == INTERMEDIO){
        aumentoX = 21;
        anchoMarco = 18;
        AltoMarco = 16;
    }

    if (dificultad == EXPERTO){
        aumentoX = 20;
        anchoMarco = 19;
        AltoMarco = 12;
    }

    BorraMarco(anchoMarco, AltoMarco, x, y);
    gotoxy ( (direccion == FLECHA_IZQ ? x-=aumentoX : x+=aumentoX), y );
    ImprimeMarco(anchoMarco, AltoMarco, x, y);
    gotoxy(x, y);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void MoverPosicionTablero_Vertical(int x, int &y, int dificultad, int direccion)
{
    int aumentoY, anchoMarco, AltoMarco;

    if (dificultad == PRINCIPIANTE){
        aumentoY = 20;
        anchoMarco = 31;
        AltoMarco = 18;
    }

    if (dificultad == INTERMEDIO){
        aumentoY = 19;
        anchoMarco = 18;
        AltoMarco = 16;
    }

    if (dificultad == EXPERTO){
        aumentoY = 13;
        anchoMarco = 19;
        AltoMarco = 12;
    }

    BorraMarco(anchoMarco, AltoMarco, x, y);
    gotoxy (x, (direccion == FLECHA_ARRIBA ? y-=aumentoY : y+=aumentoY) );
    ImprimeMarco(anchoMarco, AltoMarco, x, y);
    gotoxy(x, y);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
void ImprimeMarcador(int &marcador, int jugador)
{
    int x, y = 50;
    if (jugador == 1){
        EstableceColor(BLANCO, ROJOCLARO);
        x = 50;
    }else{
        EstableceColor(BLANCO, AZULCLARO);
        x = 160;
    }

    gotoxy(x,y);
    cout << marcador;

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

void MostrarCarta(int ancho, int alto, int x, int y)
{
    // Parte superior del marco
    EstableceColor(ROJOCLARO, ROJOCLARO);
    x++;
    y++;
    gotoxy(x,y);
    for(int i = 0 ; i < ancho ; ++i) cout << ' ';

    // Parte media del marco
    for(int i = 1 ; i <= alto - 2 ; ++i){
        gotoxy(x,++y);
        cout << ' ';
        for(int j = 1 ; j < ancho ; ++j) cout << ' ';
    }

    // Parte inferior del marco
    gotoxy(x,++y);
    for(int i = 0 ; i < ancho ; ++i) cout << ' ';
    EstableceColor(BLANCO, ROJOCLARO);

}


