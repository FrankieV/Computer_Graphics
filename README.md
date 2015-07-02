o	Trama

Frank Villella, scaraventato all’inferno per non aver superato alcuni esami, si ritrova a dover compiere “strane” imprese per uscire sano e salvo dalle mura di Tordor (nome “storpiato” dal Signore degli Anelli) .
L’inferno è diviso in tre stanze e ognuna di esse nasconde delle insidie che mettono a dura prova Frank. Quest’ultimo deve rimanere in vita fino al raggiungimento della stanza numero tre entro un tempo prestabilito ed impossessarsi del “drago rotante”. 

Informazioni Tecniche e Sviluppo

IN HELL è stato sviluppato con GLUT (OpenGL Utility Toolkit)/Freeglut, alternativa open source di OpenGL (Open Graphics Library) (ver 4.0.0 - Build 10.18.10.3308 in questo progetto), utilizzando il linguaggio C/C++.
Il gioco è composto da più stanze suddivise da pareti dove, all’interno, vi sono diverse tipologie di “trappole” da evitare. Lo spawn del giocatore avviene nella prima stanza (il luogo di spawn avviene sempre nello stesso punto). Il giocatore, ha inizialmente un livello di health di 100% che diminuisce quando avviene una collisione con una “trappola”, che assume posizioni casuali (rispettando le dimensioni della stanza). Lo scopo del gioco è riuscire a raggiungere l’ultima stanza con un livello di health maggiore o uguale all’ 1% entro un tempo T, se queste condizioni non sono soddisfatte, avviene il respawn (esattamente nella stessa posizione dove è avvenuto lo spawn).

Per il pavimento, per le pareti e per alcuni oggetti sono state utilizzate delle texture 2D reperite sul web, alcune fonti:

1.	http://www.katsbits.com/download/textures/

2.	http://www.turbosquid.com/Search/Texture-Maps/free/tga

Le texture utilizzate sono in formato TGA. Alcune texture sono state scaricate in formato JPG e poi convertite in formato TGA.
La funzione loadTexture si occupa di caricare le diverse texture e in particolare è stata utilizzata la funzione glGenTextures per caricare più texture con diverso nome.
Le stanze sono tutte dotate di un effetto nebbia ( GL_FOG )  con bassa intensità.
Quando il giocatore viene colpito da un oggetto, viene applicato un suono e un effetto nebbia rapido (di colore rosso) per far intuire al giocatore che è stato colpito. 

o	VISUAL STUDIO 

Visual Studio è un ambiente di sviluppo creato da Microsoft.
La versione utilizzata per la realizzazione del progetto è Visual Studio Community 2013 (Update 4).

o	NUGET

Nuget  è un tool integrato in Visual Studio che permette di aggiungere facilmente librerie al progetto che si sta sviluppando.
Nuget Package Manager è direttamente accessibile dal menu Tools di Visual Studio e dispone di un proprio motore di ricerca integrato per trovare ed installare automaticamente le librerie necessarie. E’ inoltre in grado di visualizzare le librerie attualmente presenti nel progetto.


o	Uso del joystick

Il gioco permette l’utilizzo del joystick per muovere il giocatore.
Per le funzionalità del joystick ho scelto di implementare la funzione glutJoystickFunc messa a disposizione da GLUT stesso per garantire la massima portabilità.

o	Uso della tastiera

E’ implementata la classica funzione glutKeyboardFun per gestire i movimenti del giocatore

o	La libreria SDL

SDL (Simple DirectMedia Layer) è una libreria open-source e cross-platform, scritta in C, che permette l’accesso all’ audio, alla tastiera, al mouse, al joystick e all’hardware grafico attraverso OpenGL.

Come riportato sul sito ufficiale (https://www.libsdl.org/), la libreria è attualmente supportata dai seguenti sistemi:
SDL officially supports Windows, Mac OS X, Linux, iOS, and Android. Support for other platforms may be found in the source code.
IN HELL utilizza tale libreria per gestire l’audio, in particolare, per questo progetto, è stata utilizzata la versione SDL 1.2, poiché sono stati riscontrati alcuni problemi con l’attuale versione (SDL 2.0).
La libreria è scaricabile dal seguente sito web:
	https://www.libsdl.org/download-2.0.php
In particolare è stata implementata la libreria SDL_Mixer v. 1.2 poiché strettamente collegata con SDL 1.2. 
SDL_Mixer è reperibile qui:
	https://www.libsdl.org/projects/SDL_mixer/release-1.2.html
In particolare, è stata implementata la funzione loadAudio per leggere i file musicali in formato WAV (ma SDL_Mixer supporta anche molti altri formati, tra cui MP3).

o	GitHub

GitHub è un servizio web di hosting per lo sviluppo di progetti software, che usa il sistema di controllo di versione Git. 
Il codice sorgente del gioco è disponibile all’indirizzo:

1. https://github.com/FrankieV/Computer_Graphics
