/*
 * =====================================================================================
 *
 *       Filename:  steampunkffr.c
 *
 *    Description:  Steampunk FFR - Der Anfang
 *    				Ein "Das-ist-dein-Abenteuer"-Roman
 *    				Dieser Quelltext versucht die Fähigkeiten von C auszuschöpfen, daher
 *    				ist C99 oder neuer notwendig, um ihn zu kompilieren.
 *
 *        Version:  0.029
 *    letzte Beta:  0.000
 *        Created:  22.05.2011 09:35:00
 *          Ended:  00.00.0000 00:00:00
 *       Revision:  none
 *       Compiler:  clang
 *        compile:  clang -o steampunkffr toolbox.c steampunkffr.c -lncurses
 *
 *         Author:  Sascha K. Biermanns (saschakb), saschakb@gmail.com
 *        Company:
 *        License:  ISC
 *
 *   Copyright (C)  2011, Sascha K. Biermanns
 *
 * ====================================================================================
 *
 *   Permission to use, copy, modify, and/or distribute this software for any
 *   purpose with or without fee is hereby granted, provided that the above
 *   copyright notice and this permission notice appear in all copies.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *   WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *   ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * =====================================================================================
 *
 *   Letze Änderungen:
 *   - 22.05.2011 Beginn an der Arbeit des Spiels
 *   - 22.05.2011 Schusswaffen und Munition implementiert
 *   - 22.05.2011 Tripodenauftauchen eingebaut
 *   - 24.06.2011 Eine Auswahl fehlte bei ort1()
 *   - 24.06.2011 Beginne damit, eine erweiterten Stadtplan einzusetzen
 *   - 25.06.2011 Mehr Action und das Auftauchen der Dreistelzer wird eingearbeitet
 *   - 25.06.2011 Der Storyabschnitt für Mönchengladbach ist soweit beendet
 *   - 25.06.2011 Hohlwelt Eingangseben - Beginn am Labyrinth zu arbeiten
 *   - 26.06.2011 Labyrinth und Geheimgänge der Eingangsebene Hohlwelt vollständig
 *   - 26.06.2011 Der Drache ist jetzt im Spiel ^.^
 *   - 28.06.2011 Die Funktion auswahl() wurde von 6 möglichen Auswahlen auf eine
 *                VA-Liste umgestellt. Somit stehen viel mehr Möglichkeiten offen.
 *   - 02.07.2011 Beendigung der Arbeit an der Zwergenmine
 *   - 02.07.2011 Beschreibungen der Labyrinthwege
 *   - 02.07.2011 Eine Funktion für Versrätsel wurde hinzugefügt
 *   - 03.07.2011 Die Eingangsebene der Hohlwelt ist fertig
 *   - 04.07.2011 Beginn an der Storyline der Dwellmer
 *   - 12.09.2011 Splittung des Moduls in einen technischen Teil (toolbox.X) und den
 *                spielerischen Teil
 *                Optimierung einiger Routinen
 *                Minimierung eingebundener Quellen
 *                Veränderung der laden()-/speichern()-Routinen
 *   - 13.09.2011 texteingabe() separiert von textausgabe() - Prinzip: Vereinfachung 
 *                beenden() hinzugefügt - Prinzip: Vereinfachung
 *                hinweis() hinzugefügt - Prinzip: Vereinfachung
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h> // Für die VA-Liste
#include "toolbox.h"

#define DATEINAME ".steampunkffrsicherung.txt"

// Neuer Typ: charakter_t & npc_t
// Zur Speicherung der Kampfwerte

typedef struct _charakter {
	char name[30];
	int gewandheit;
	int gewandheit_start;
	int staerke;
	int staerke_start;
	int glueck;
	int glueck_start;
} charakter_t;

// -----------
// Der Spieler
// -----------
static charakter_t spieler;

// ----------------------
// Variablen des Spielers
// ----------------------

enum objektsynonyme { nichts, adamantpickel, bogensilberpfeil, buch, drachenfeuerzauber, gewandheitstrank, gewehr, glueckstrank, goldstuecke, handschuh, helm, holzhammer, holzpflock, holzschild, juwelenauge, kaese, kerze, laterne, lederruestung, lederruestung2, ohrringe, patrone, pergament, pistole, proviant, rucksack, schild, schwert, schwert2, silberkreuz, silbersichel, staerketrank, taschenlampe, taschenmesser, unsichtbarkeitstrank, verbandskasten, verwuenschterhelm, warndreieck, wunderwasser, zombiegold ,maximalobjekt };

static int objekt[maximalobjekt] = { [laterne] = 1, [lederruestung] = 1, [proviant] = 10, [rucksack] = 1, [schwert] = 1 };
static char *objektname[maximalobjekt] = { "Nichts", "Adamantpickel", "Bogen und Silberpfeil", "Buch", "Drachenfeuerzauber", "Gewandheitstrank", "Gewehr", "Glückstrank", "Goldstücke", "Handschuh", "Helm", "Holzhammer", "Holzpflock", "Holzschild", "Juwelenauge", "Käse", "blaue Kerze", "Laterne", "Lederrüstung", "Lederrüstung", "Ohrringe", "Patronen", "Pergament", "Pistole", "Proviant", "Rucksack", "Schild", "Schwert", "Zauberschwert +2", "Silberkreuz", "Silbersichel", "Stärketrank", "Pumptaschenlampe", "Taschenmesser", "Unsichtbarkeitstrank", "Verbandskasten", "Verwuenschter Helm", "Warndreieck",  "Wunderwasser", "Zombiegold" };

static int angriffsbonus = 0;
static int fuenfwahl = 0;
static bool nursilbertrifft = false;
static int paralysiert = 0;
static int preis = 0;
static int schluessel = 0;
static bool schluessel9 = false;
static bool schluessel66 = false;
static bool schluessel99 = false;
static bool schluessel111_1 = false;
static bool schluessel111_2 = false;
static bool schluessel125 = false;
static bool schluesselbootshaus = false;
static bool schummeln = false;
static bool silberwaffe = false;
static bool unsichtbar = false;
static bool tripodgesehen = false;
static int getoetetemenschen = 0;
static int getoetetegegner = 0;
static bool agartha = false;
static bool verzeichnisgelesen = false;
static bool buchgefunden = false;
static bool kartegefunden = false;
static bool sargverschoben = false;
static bool durchganggeoeffnet = false;
static bool schluesselgefunden = false;
static bool dreistelzer = false;
static bool dracheverletzt = false;
static bool drachetot = false;
static int minenzwerge = 158;
static int stollentroll = 150;
static bool gitteroffen = false;
static bool raetsel1 = false;
static bool raetsel2 = false;
static bool raetsel3 = false;
static bool raetsel4 = false;
static bool raetsel5 = false;
static bool dwellmer = false;
static int arianna = 0;
static int elke = 0;
static bool schluesselarianna = false;
static int verloben = 0;

static unsigned int rotation = 0; // Rotation ist eine Besonderheit. Hierüber werden die beiden Drehraumsegmente gesteuert. ^.^

// -------------
// Raum Variable
// -------------

static int raum = 0;

/*
// ----------------
// Beenden Variable
// ----------------

bool spielbeenden = false;
*/

// -----------------------------------------
// Spezialmonster (immun außer gegen Silber)
// -----------------------------------------

charakter_t mechanicher_geher = { "mechanicher Geher", 14, 13, 12, 12 };

// --------------
// externe Module
// --------------

extern void beenden(enum farben f, int status, char* text, ...); // Funktion: Farbige Beenden-Funktion

extern void hintergrundfarbe(enum farben); // Funktion: Hintergrundfarbe ändern

extern void hinweis(enum farben f, char* text, ...); // Funktion: Hinweis - für Fehlermeldungen oder ähnliches

extern bool janeinfrage(char *); // Funktion: Ja-Nein-Frage

extern void ncurses_init(void (*)()); // Initialisierung der ncurses-Umgebung

extern char taste(void); // Funktion: Taste

extern void textausgabe(char *, ...); // Funktion: Textausgabe

extern void texteingabe(char *, unsigned int); // Funktion: Texteingabe

extern void vordergrundfarbe(enum farben); // Funktion: Vordergrundfarbe ändern

extern int waehle(char*, int); // Funktion: waehle

extern void weiter(void); // Funktion: Weiter

extern int wuerfel(unsigned int); // Funktion: Wuerfel

extern void zufall_per_zeit(void); // Initialisierung der Zufallszahlen

// -------------------
// Funktionsprototypen
// -------------------

// Funktion: Teste dein Glück
bool tdg(charakter_t *);

// Funktion: Kampfrunde
bool kampfrunde(charakter_t *, charakter_t *, void (*)());

// Funktion: Kampf
bool kampf(charakter_t *, charakter_t *, int, bool, void (*)());

// Funktion: Momentane Werte
void momentane_werte(charakter_t *);

// Funktion: Auswahl
void auswahl(char *, int, ...);

// Funktion: VersucheDeinGlueck
void versuchedeinglueck(void (*)(), void (*)());

// Funktion: VersucheDeineGewandheit
void versuchedeinegewandheit(void (*)(), void (*)());

// Funktion: Flucht
void flucht(void (*funktion1)());

// Funktion: Mahlzeit
void mahlzeit(void);

// Funktion: Gewandheitstrank trinken
void gewandheitstrank_trinken();

// Funktion: Stärketrank trinken
void staerketrank_trinken();

// Funktion: Glückstrank trinken
void glueckstrank_trinken();

// Funktion: Objkekt ablegen
void objekt_ablegen();

// Funktion: gewandheitssteigerung
void gewandheitssteigerung(int, int);

// Funktion: staerkesteigerung
void staerkesteigerung(int, int);

// Funktion: glueckssteigerung
void glueckssteigerung(int, int);

// Funktion: speichern
int speichern(void);

// Funktion: laden
int laden(void);

// Funktion: quit
void quit();

// Funktion: Rätsel
bool raetsel(char *raetseltext, char *antworttext);

// Funktion: Zweisamkeit
void zweisamkeit(int);

// ---------------------------------
// Implementation für das Spielmodul
// ---------------------------------

void vorwort();
void ort1();
void ort2();
void ort3();
void ort4();
void ort5();
void ort6();
void ort7();
void ort8();
void ort9();
void ort10();
void ort11();
void ort12();
void ort13();
void ort14();
void ort15();
void ort16();
void ort17();
void ort18();
void ort19();
void ort20();
void ort21();
void ort22();
void ort23();
void ort24();
void ort25();
void ort26();
void ort27();
void ort28();
void ort29();
void ort30();
void ort31();
void ort32();
void ort33();
void ort34();
void ort35();
void ort36();
void ort37();
void ort38();
void ort39();
void ort40();
void ort41();
void ort42();
void ort43();
void ort44();
void ort45();
void ort46();
void ort47();
void ort48();
void ort49();
void ort50();
void ort51();
void ort52();
void ort53();
void ort54();
void ort55();
void ort56();
void ort57();
void ort58();
void ort59();
void ort60();
void ort61();
void ort62();
void ort63();
void ort64();
void ort65();
void ort66();
void ort67();
void ort68();
void ort69();
void ort70();
void ort71();
void ort72();
void ort73();
void ort74();
void ort75();
void ort76();
void ort77();
void ort78();
void ort79();
void ort80();
void ort81();
void ort82();
void ort83();
void ort84();
void ort85();
void ort86();
void ort87();
void ort88();
void ort89();
void ort90();
void ort91();
void ort92();
void ort93();
void ort94();
void ort95();
void ort96();
void ort97();
void ort98();
void ort99();
void ort100();
void ort101();
void ort102();
void ort103();
void ort104();
void ort105();
void ort106();
void ort107();
void ort108();
void ort109();
void ort110();
void ort111();
void ort112();
void ort113();
void ort114();
void ort115();
void ort116();
void ort117();
void ort118();
void ort119();
void ort120();
void ort121();
void ort122();
void ort123();
void ort124();
void ort125();
void ort126();
void ort127();
void ort128();
void ort129();
void ort130();
void ort131();
void ort132();
void ort133();
void ort134();
void ort135();
void ort136();
void ort137();
void ort138();
void ort139();
void ort140();
void ort141();
void ort142();
void ort143();
void ort144();
void ort145();
void ort146();
void ort147();
void ort148();
void ort149();
void ort150();
void ort151();
void ort152();
void ort153();
void ort154();
void ort155();
void ort156();
void ort157();
void ort158();
void ort159();
void ort160();
void ort161();
void ort162();
void ort163();
void ort164();
void ort165();
void ort166();
void ort167();
void ort168();
void ort169();
void ort170();
void ort171();
void ort172();
void ort173();
void ort174();
void ort175();
void ort176();
void ort177();
void ort178();
void ort179();
void ort180();
void ort181();
void ort182();
void ort183();
void ort184();
void ort185();
void ort186();
void ort187();
void ort188();
void ort189();
void ort190();
void ort191();
void ort192();
void ort193();
void ort194();
void ort195();
void ort196();
void ort197();
void ort198();
void ort199();
void ort200();
void ort201();
void ort202();
void ort203();
void ort204();
void ort205();
void ort206();
void ort207();
void ort208();
void ort209();
void ort210();
void ort211();
void ort212();
void ort213();
void ort214();
void ort215();
void ort216();
void ort217();
void ort218();
void ort219();
void ort220();
void ort221();
void ort222();
void ort223();
void ort224();
void ort225();
void ort226();
void ort227();
void ort228();
void ort229();
void ort230();
void ort231();
void ort232();
void ort233();
void ort234();
void ort235();
void ort236();
void ort237();
void ort238();
void ort239();
void ort240();
void ort241();
void ort242();
void ort243();
void ort244();
void ort245();
void ort246();
void ort247();
void ort248();
void ort249();
void ort250();
void ort251();
void ort252();
void ort253();
void ort254();
void ort255();
void ort256();
void ort257();
void ort258();
void ort259();
void ort260();
void ort261();
void ort262();
void ort263();
void ort264();
void ort265();
void ort266();
void ort267();
void ort268();
void ort269();
void ort270();
void ort271();
void ort272();
void ort273();
void ort274();
void ort275();
void ort276();
void ort277();
void ort278();
void ort279();
void ort280();
void ort281();
void ort282();
void ort283();
void ort284();
void ort285();
void ort286();
void ort287();
void ort288();
void ort289();
void ort290();
void ort291();
void ort292();
void ort293();
void ort294();
void ort295();
void ort296();
void ort297();
void ort298();
void ort299();
void ort300();
void ort301();
void ort302();
void ort303();
void ort304();
void ort305();
void ort306();
void ort307();
void ort308();
void ort309();
void ort310();
void ort311();
void ort312();
void ort313();
void ort314();
void ort315();
void ort316();
void ort317();
void ort318();
void ort319();
void ort320();
void ort321();
void ort322();
void ort323();
void ort324();
void ort325();
void ort326();
void ort327();
void ort328();
void ort329();
void ort330();
void ort331();
void ort332();
void ort333();
void ort334();
void ort335();
void ort336();
void ort337();
void ort338();
void ort339();
void ort340();
void ort341();
void ort342();
void ort343();
void ort344();
void ort345();
void ort346();
void ort347();
void ort348();
void ort349();
void ort350();
void ort351();
void ort352();
void ort353();
void ort354();
void ort355();
void ort356();
void ort357();
void ort358();
void ort359();
void ort360();
void ort361();
void ort362();
void ort363();
void ort364();
void ort365();
void ort366();
void ort367();
void ort368();
void ort369();
void ort370();
void ort371();
void ort372();
void ort373();
void ort374();
void ort375();
void ort376();
void ort377();
void ort378();
void ort379();
void ort380();
void ort381();
void ort382();
void ort383();
void ort384();
void ort385();
void ort386();
void ort387();
void ort388();
void ort389();
void ort390();
void ort391();
void ort392();
void ort393();
void ort394();
void ort395();
void ort396();
void ort397();
void ort398();
void ort399();
void ort400();

// --------------------------------
// Zeigerliste aller ort-Funktionen
// --------------------------------
void (*raumptr[401]) (void) = {vorwort, ort1, ort2, ort3, ort4, ort5, ort6, ort7, ort8, ort9, ort10, ort11, ort12, ort13, ort14, ort15, ort16, ort17, ort18, ort19, ort20, ort21, ort22, ort23, ort24, ort25, ort26, ort27, ort28, ort29, ort30, ort31, ort32, ort33, ort34, ort35, ort36, ort37, ort38, ort39, ort40, ort41, ort42, ort43, ort44, ort45, ort46, ort47, ort48, ort49, ort50, ort51, ort52, ort53, ort54, ort55, ort56, ort57, ort58, ort59, ort60, ort61, ort62, ort63, ort64, ort65, ort66, ort67, ort68, ort69, ort70, ort71, ort72, ort73, ort74, ort75, ort76, ort77, ort78, ort79, ort80, ort81, ort82, ort83, ort84, ort85, ort86, ort87, ort88, ort89, ort90, ort91, ort92, ort93, ort94, ort95, ort96, ort97, ort98, ort99, ort100, ort101, ort102, ort103, ort104, ort105, ort106, ort107, ort108, ort109, ort110, ort111, ort112, ort113, ort114, ort115, ort116, ort117, ort118, ort119, ort120, ort121, ort122, ort123, ort124, ort125, ort126, ort127, ort128, ort129, ort130, ort131, ort132, ort133, ort134, ort135, ort136, ort137, ort138, ort139, ort140, ort141, ort142, ort143, ort144, ort145, ort146, ort147, ort148, ort149, ort150, ort151, ort152, ort153, ort154, ort155, ort156, ort157, ort158, ort159, ort160, ort161, ort162, ort163, ort164, ort165, ort166, ort167, ort168, ort169, ort170, ort171, ort172, ort173, ort174, ort175, ort176, ort177, ort178, ort179, ort180, ort181, ort182, ort183, ort184, ort185, ort186, ort187, ort188, ort189, ort190, ort191, ort192, ort193, ort194, ort195, ort196, ort197, ort198, ort199, ort200, ort201, ort202, ort203, ort204, ort205, ort206, ort207, ort208, ort209, ort210, ort211, ort212, ort213, ort214, ort215, ort216, ort217, ort218, ort219, ort220, ort221, ort222, ort223, ort224, ort225, ort226, ort227, ort228, ort229, ort230, ort231, ort232, ort233, ort234, ort235, ort236, ort237, ort238, ort239, ort240, ort241, ort242, ort243, ort244, ort245, ort246, ort247, ort248, ort249, ort250, ort251, ort252, ort253, ort254, ort255, ort256, ort257, ort258, ort259, ort260, ort261, ort262, ort263, ort264, ort265, ort266, ort267, ort268, ort269, ort270, ort271, ort272, ort273, ort274, ort275, ort276, ort277, ort278, ort279, ort280, ort281, ort282, ort283, ort284, ort285, ort286, ort287, ort288, ort289, ort290, ort291, ort292, ort293, ort294, ort295, ort296, ort297, ort298, ort299, ort300, ort301, ort302, ort303, ort304, ort305, ort306, ort307, ort308, ort309, ort310, ort311, ort312, ort313, ort314, ort315, ort316, ort317, ort318, ort319, ort320, ort321, ort322, ort323, ort324, ort325, ort326, ort327, ort328, ort329, ort330, ort331, ort332, ort333, ort334, ort335, ort336, ort337, ort338, ort339, ort340, ort341, ort342, ort343, ort344, ort345, ort346, ort347, ort348, ort349, ort350, ort351, ort352, ort353, ort354, ort355, ort356, ort357, ort358, ort359, ort360, ort361, ort362, ort363, ort364, ort365, ort366, ort367, ort368, ort369, ort370, ort371, ort372, ort373, ort374, ort375, ort376, ort377, ort378, ort379, ort380, ort381, ort382, ort383, ort384, ort385, ort386, ort387, ort388, ort389, ort390, ort391, ort392, ort393, ort394, ort395, ort396, ort397, ort398, ort399, ort400 };

// -------------------------
// Das eigentliche Abenteuer
// -------------------------
void intro(void) {
	int eingabe = 0;

    vordergrundfarbe(gelb);
    textausgabe("Hinweis!");
	vordergrundfarbe(gruen);
	textausgabe("In diesem Roman wird niemand anderes als du selbst die Person sein, die das Abenteuer durchlebt. Von daher würde ich sagen, würdest du dir selber beim Erleben der Spielatmosspähre helfen, wenn du die Spielfigur nach dir benennst, oder ihr einen Namen gibst, der dir gefällt oder den du gerne tragen würdest.\nViel Spaß beim Lesen und Spielen!\n");
    vordergrundfarbe(blau);
    textausgabe("Sascha\n");
	vordergrundfarbe(zyan);
    textausgabe("Welchen Namen möchtest du deinem Abenteurer geben? ");
    vordergrundfarbe(gelb);
    texteingabe(spieler.name, 30);
    vordergrundfarbe(weiss);
	spieler.gewandheit_start = wuerfel(6) + 6;
	spieler.gewandheit = spieler.gewandheit_start;
	spieler.staerke_start = wuerfel(6) + wuerfel(6) + 12;
	spieler.staerke = spieler.staerke_start;
	spieler.glueck_start = wuerfel(6) + 6;
	spieler.glueck = spieler.glueck_start;
    vordergrundfarbe(gruen);
	textausgabe("\nZu Beginn dieses Abenteuer wirst du nur ein absolutes Minimum an Objekten bei dir führen, als da wären ein Rucksack, ein Multifunktionstaschenmesser, eine Pumptaschenlampe und etwas Proviant. Außerdem ein Engergydrink, den du dir selber auswählen kannst. Welchen der Energydrinks wählst du?\n(1) den Gewandheitstrank - er stellt die anfängliche Gewandheitspunktzahl wieder her\n(2) den Stärketrank - er stellt die anfängliche Stärkepunktzahl wieder her\n(3) den Glückstrank - er stellt die anfängliche Glückspunktzahl wieder her und verbesser sie zusätzlich um einen Punkt");
	while((eingabe < 1) || (eingabe > 3)) {
        vordergrundfarbe(zyan);
		eingabe = waehle("Für welchen Energydrink entscheidest du dich? ", 3);
        vordergrundfarbe(weiss);
		switch(eingabe) {
			case 1: objekt[gewandheitstrank] += 2;
					break;
			case 2: objekt[staerketrank] += 2;
					break;
			case 3: objekt[glueckstrank] += 2;
					break;
			default: vordergrundfarbe(rot);
					 textausgabe("Unerklärbarer Fehler!\nIn der Energydrink-Auswahl,  in Funktion intro().");
					 vordergrundfarbe(weiss);
					 break;
		}
	}
    vordergrundfarbe(gruen);
    textausgabe("\nSo möge es denn sein! Du hast deine Wahl getroffen. Viel Spaß!\n");
    vordergrundfarbe(weiss);
    weiter();
}

void vorwort(void) {
	textausgabe("Vorwort\n\nViele Menschen würden der Wiedervereinigung und der Politik die Schuld geben, das Menschen ins Abseits geraten. Die meisten wissen nicht, was das wirklich bedeutet, du jedoch schon. Seit Jahren bereits kämpfst du um dein Überleben, mit nicht mehr als deiner Kleidung am Körper, einem Ruckscack, einem Zelt, einer Pumptaschenlampe, einem Multifunktionstaschenmesser billigster Machart und einigen wenigen weiteren Kleinigkeiten. Momentan ist das Wetter noch Mild - und so hast du die Nacht in den Überresten einer alten Turmruine verbracht, die unbeachtet im Park des Museums am Münster steht.\nDoch diese Nacht war anders, als jede andere Nacht, die du im Freien verbracht hast. Sie war unheimlich. Du kannst dich erinnern, wie du aus dem Zelt klettertest - aber es war noch dunkel, und pechschwarz. Der ganze Himmel, alles war nur Schwarz, kein einziger Stern war zu sehen, nicht die Silhouette des Mondes, keine Wolken - rein gar nichts! Und dann war da dieses röhrende, metallische Geräusch. Und noch bevor du weißt, wie dir geschieht, wird es langsam hell - und du stehst immer noch da, in der gleichen Position. Bewegungslos. Die Frage ist nur, wie lange schon hast du bewegungslos herumgestanden? Und wieso überhaupt konntest du dich nicht bewegen?\nDa es noch immer recht früh ist, gehst du hinunter zu dem kunstvollen Brunnen des Museums. Der Park gehört in diesen Stunden dir allein. Du nimmst ein kurzes Bad und fühlst dich direkt viel wohler, so sauber, wenn es auch ein klein wenig eiskalt war. Mit einem alten Handtuch reibst du dich notdürftig trocken und ziehst dir saubere Unterwäsche an, bevor der Rest deiner alltäglichen Kleidung folgt. Den Rest deiner Kleidung verstaust du in einem Stoffbeutel und gehst zurück zu deinem Zelt. Mit wenigen Handgriffen baust du es auseinander, dann faltest du es und verstaust es vorsichtig.\nMit deinem ganzen Habseligkeiten bepackt gehst du die Treppen des Münster hinunter. Als Kind hast du dafür immer den schnellen Weg gewählt, mit den glatten Sandahlensohlen seid deine Freunde und ihre einfach immer stehend das Geländer hinuntergerutscht. Kinder kenen keinerlei Angst vor Gefahren. Du überquerst den Platz hinüber in Richtung des ehemaligen Dortin-Hotels und schlägst den Weg, die Hügelstraße hinauf ein. Nahe dem alten Wasserturm am Odenkirchweg versteckst du den Rucksack mit deinem Zelt, der Isoliermatte und all den anderen Dingen, die du nur des Nachts brauchst - und wanderst nur mit deinem \"leichten Gepäck\" die Vitusstraße hinunter. Du wanderst die Lüperzender Straße entlang und gehst schließlich die Brücke am ehemaligen Zentralbad hinauf - um noch kurz den kleine Park zu durchqueren, bevor du in Kürze die Hindenburgstraße nahe des Stadttheaters, oder einem seiner Nachfahren erreichst.");
}

// -----------
// --- 001 ---
// -----------

void ort1(void) {
	raum = 1;
	textausgabe("Die Innenstadt macht um diese Zeit noch einen gänzlich unbelebten Eindruck - und kein einiges Licht erhellt die Straße. Keine Straßenlaterne, keine Glühbirne hinter einem Fenster, nicht einmal die Werbetafeln leuchten auf. Wenn du so drüber nachdenkst, auf deinem ganzen Weg hierhin hast du bisher keinerlei eingeschaltete Lichtquellen gesehen.\nNun stehst du an der Kreuzung Stepgesstraße Ecke Hindenburgstraße. Ein Stück die große Einkaufsstraße hinauf ist das Bäckereigeschäft, an dem dir jeden Morgen ein guter Geist eine braune Papiertüte mit dampfend warmen Gebäck bereitstellt. Du schaust in die Richtung, doch diesmal steht auf dem Stromkasten neben der Türe nicht der verlockende Beutel, sondern eine Gestalt steht in der Türe und winkt dir zu.");
	auswahl("Wirst du auf die winkende Gestalt zugehen (1) oder ziehst du es vor, dich lieber scheu und mit knurrendem Magen aus dem Staub zu machen (2)?", 2, ort2, ort3);
}

void ort2(void) {
	raum = 2;
	textausgabe("Vorsichtig und etwas nervös näherst du dich der offenstehenden Ladentüre, als ein Regelrechter Platzregen losbricht, dann hört man es auch donnern. Jetzt läufst du auf die Türe zu. Eine Frau im weißen Kitel geht hinein und hält sie dir offen. Du gleitest hinein und bleibst neben einem Stehtisch stehen, während sie hinter dir die Türe zumacht und abschließt.\n\"Ist das nicht ein unheimlicher Morgen? Die ganze Stadt ist ohne Strom. Momentan habe ich ein Feuer im alten Backofen gemacht - gleich wird es hier ein paar frische Brötchen geben.\" sagt sie und geht an der Theke vorbei auf eine Tür die nach hinten führt zu. \"Na kommen sie!\" lädt sie dich ein, ihr zu folgen. Du gehst unsicher hinterher. Im Hinterzimmer steht ein Tisch, auf dem eine Thermoskanne und zwei Tassen bereitstehen. \"Ich wollte sie schon längst einmal angesprochen haben. Daß das an einem so schlechten Tag passiert, tut mir leid!\" Sie kommt mit einem Tablet voller dampfender Brötchen aus einem Nebenraum, während du unschlüssig neben dem Tisch stehst.\n\"Bitte setzen sie sich doch! Ich dachte, wir könnten zusammen frühstücken!\" sagt sie und stellt das Tablett auf dem Tisch ab. Dann setzt sie sich hin. \"Ich bin Elke.\" stellt sie sich vor.\nAls du ihr gerade mit deinem Namen antworten willst, geschieht das unfassbare: die Sirenen fangen an zu heulen, mitten in dieser Kackophonie des Gewitters - und das um Fünf Uhr, Morgens in der Frühe.\n\"Was geht da vor sich?\" fragt Elke, während du dir das Sirenensignal genauer anhörst. \"Haben sie ein Radio hier, oder einen Fernsehe?\" fragst du Elke, aber die zuckt nur mit den Schultern.\n\"Kein Batteriebetriebenes, der Strom ist ausgefallen, da tut's der Fernseher nicht.\" Sie deutete mit der Hand neben deinem Ohr entlang hinter dich. Du drehst dich auf der Sitzfläche des Stuhls um und erblickst das Gerät. Die Sirenen heulen jetzt bereits seit mehr als einer Minute.\n\"Das ist keine Übung, das ist eine echte Warnung, aber vor was?\" sagst du laut zu dir. Du sprichst sehr oft mit dir selbst, vermutlich, um nicht vor Einsamkeit den Verstand zu verlieren. Wieder erklingt das Donnern und Grollen.");
	auswahl("Willst du erst einmal in Ruhe Frühstücken (1) oder Elke fragen, ob sie dich rauslassen kann, damit du nachsiehst, was eigentlich los ist (2) oder sie fragen, ob sie über einen weiteren Empfänger verfügt, der nicht ans Stromnetz angeschlossen ist (3)?", 3, ort4, ort5, ort6);
}

void ort3(void) {
// Kein Kontakt
	raum = 3;
	textausgabe("Du bist ein äußerst mißtrauischer Mensch und vertraust der unbekannten, winkenden Gestalt nicht. Zu deinem Glück fängt es an zu regnen. Es ist ein echter Platzregen. Du rennst ein Stück weit die Hindenburgstraße hinab und biegst in die Lichtpassage ein, wo du dich während des Regens unterstellts. Kaum bist du eine Minute drin, als plötzlich überall die Sirenen anfangen zu heulen. Draußen hörst du ein lautes Donnergrollen. Auch ist es viel dunkler geworden, seit die dichten grauen Wolken sich mit ihrem Regen auf Gladbach ergehen. Plötzlich pfeift ein Schuß an dir vorbei und schlägt in die Fensterscheibe ein Stück vor dir ein. Du hechtest in den nächsten Eingang und siehst zurück. Du bist nicht alleine. Von oben herab kommt eine Gestalt gelaufen. Aus deinem Rucksack holst du dein Taschenmesser heraus und klappst es auf. Es ist fast so, als wärst du unbewaffnet, aber besser als nichts. Dein Gegner kommt weiter auf dich zugeeilt.");
	ort13(); // Zufallsgegner
	textausgabe("Die Pistole deines Gegners ist leergeschossen. Du durchsuchst ihn hastig, findest, aber keinerlei Munition bei ihm.");
	if(janeinfrage("Möchtest du die Pistole einstecken (j/n)?"))
		objekt[pistole] += 1;
	textausgabe("Es war zwar Notwehr, dennoch hockst du jetzt hier neben einer Leiche. Dein Gefühl sagt dir, daß es wohl das beste wäre, dich so schnell wie möglich aus dem Staub zu machen, Unwetter hin oder her.");
	auswahl("Möchtest du den Lichthof nach Norden verlassen in Richtung Kaisterstraße (1), oder nach Süden in Richtung Hindernburgstraße (2)?", 2, ort50, ort51);
}

void ort4(void) {
// Frühstücken
	raum = 4;
	textausgabe("Du setzt dich zu Elke an den Tisch. Das Wetter, den Donner und den fehlenden Strom ignorieren, lernt ihr euch langsam besser kennen, während die Anzahl der Backwaren auf dem Tablett deutlich schrumpfen und der heiße Kaffee aus der Thermoskanne sich wohlig in deinem Inneren verteilt. Der Ort, die Zeit, die Situation, alles könnte man als idyllisch bezeichnen, wenn, ja wenn nicht auf einmal dieses seltsame Geräusch eine absterbenden Sirene gewsen wäre. Es war kein Abschwellen, wie man es kenn, sondern klang eher, wie ein entenartiger Aufschrei.\nUnd dann nimmst du plötzlich war, das mit dem Verstummen dieser Sirene die Masse an Sirenen, die noch klingen, weniger hoch ist, als zuvor. Aber was viel wichtiger ist, einige gehen immer noch. Langsam wirst du dir der Situation bewußt, die da draußen herrscht - und du beschließt, nachsehen zu gehen, was da los ist.");
	ort5();
}

void ort5(void) {
// Raus auf die Straße
	raum = 5;
	textausgabe("Du schlägst vor, daß du dich draußen umsiehst und zurückkommst, sobald du weißt, was los ist. Elke begleitet dich zur Ladentüre und läßt dich raus. Der Regen prasselt von oben herab und immer wieder donnert es. Du winkst ihr kurz zu und rennst so gut es geht an den Hauswänden entlang die Hindenburgstraße hinauf, trotzdem bist du nach weniger als einer Minute bis auf die Unterhose durchnäßt. Als du am ehemaligen Heinemann vorbeikommst und durch die kurze Passage läuftst, bemerkst du an der Straßenecke zum Sankt Vith hinunter einen brennenden Polizeiwagen. Ein mulmiges Gefühl geht dir durch den Magen. Eigentlich wolltest du ja in das Haus, das früher deinem Großvater gehört hat - und von dem aus man eine Übersicht über die ganze Stadt hat. Trotzdem ergreift dich gerade die Angst.");
	auswahl("Vielleicht wäre es ja besser, die Straße wieder hinunter zulaufen - und sich im Geschäft bei Elke zu verstecken (1)? Du könntest auch auf den Polizeiwagen zulaufen, vielleicht kannst du jemandem helfen, der im Wagen eingeklemmt ist (2)? Oder du folgst deinem ursprünglichen Plan, das Haus deines Großvaters zu erreichen (3)?", 3, ort7, ort8, ort9);
}

void ort6(void) {
// Nach einem Empfänger fragen
	raum = 6;
	textausgabe("Elke schüttelt den Kopf, und verneint deine Frage. \"Tut mir leid, so etwas ist nicht hier im Geschäft. Das einzige Radio, das mir einfallen würde, wäre mein Autoradio.\" entgegnet sie, nimmt sich ein warmes Brötchen vom Tablett und kaut daran herum.");
	auswahl("Willst du sie wirklich überreden, mit dir zu ihrem Auto zu gehen (1), oder würdest du jetzt nicht viel lieber frühstücken (2)? Wenn du unbedingt draußen nachsehen willst, was los ist, könntest du Elke auch bitten, dich rauszulassen (3).", 3, ort17, ort4, ort5);
}

void ort7(void) {
// Zurück zum Geschäft
	raum = 7;
	textausgabe("Vollkommen durchnäßt kommst du wieder am Geschäft an. Drinnen ist alles dunkel. Du klopfst mehrfach, aber nichts rührt sich.");
	auswahl("Willst du es weiter mit klopfen und rufen probieren (1), oder willst du versuchen, ob du die Türe öffnen kannst (2), oder willst du dir einen anderen Weg suchen (3)?", 3, ort14, ort15, ort16, NULL, NULL, NULL);
}

void ort8(void) {
// Zum Polizeiwagen
	raum = 8;
	if(objekt[gewehr] > 0) {
		textausgabe("Du läufst erneut über den Platz um dir den Polizeiwagen noch einmal anzusehen. Das Feuer ist erloschen. Du überlegst dir gerade, ob du eine noch gründlichere Durchsuchung des Wagens vornehmen sollst, als du ein Geräusch aus der Richtung der Kirche hören kannst. Ein Geschoss schlägt in die Karosserie des Wagens ein.");
		auswahl("Willst du dich dem Kampf stellen (1) oder versuchen zu fliehen (2)?", 2, ort10, ort11);
	}
	else {
		textausgabe("Der Polizeiwagen raucht eigentlich nur noch. Fahrer- und Beifahrertüre stehen weit offen. Vorsichtig umrundest du den Wagen. Kein Mensch ist darin, aber auch kein Anzeichen der beiden Polizisten, die sich darin hätte befinden müssen. Im offenen Seitenfach der Beifahrertüre ist ein Gewehr, die Verriegelung der Waffe ist offen. Du könntest sie an dich nehmen. Du durchsuchst den Kofferraum. In diesem befindet sich ein Munitionspäckchen mit 100 Patronen, ein Verbandskasten und ein Warndreieck.");
		if(janeinfrage("Willst du etwas von den Sachen mitnehmen (j/n)?")) {
			if(janeinfrage("Willst du das Gewehr mitnehmen (j/n)?"))
				objekt[gewehr] += 1;
			if(janeinfrage("Willst du die Patronen mitnehmen (j/n)?"))
				objekt[patrone] += 100;
			if(janeinfrage("Willst du den Verbandskasten mitnehmen (j/n)?"))
				objekt[verbandskasten] += 1;
			if(janeinfrage("Willst du das Warndreieck mitnehmen (j/n)?"))
				objekt[warndreieck] += 1;
		}
		auswahl("Willst du jetzt weiter zum Haus deines Großvaters (1) oder zurück zum Geschäft (2)?", 2, ort9, ort7);
	}
}

void ort9(void) {
// Zum Haus deines Großvaters
	raum = 9;
	if(dreistelzer) {
		textausgabe("Der Kampf ist nicht spurlos am Haus deines Großvaters vorbeigezogen. Die Fenster sind zersplittert, die Haustüre liegt am Boden, die Wände rußgeschwärzt.");
		auswahl("Willst du in das Haus deines Großvaters hinein (1) oder versuchen, um die Ecke zu laufen und den Kapuzinerplatz zu erreichen (2)?", 2, ort21, ort28);
	}
	if(tripodgesehen == true) {
		textausgabe("Du läufst auf das Haus deines Großvaters zu und in den Eingang zwischen Gaststätte und Kleidungsgeschäft. Für einen Moment mußt du vor der alten Holztüre mit der Glasscheibe halten. Mit Hilfe deines Taschenmessers daudert es nur ein paar Sekunden, dann ist die Türe öffnet und du schlüpfst hinein.");
		ort21();
	}
	textausgabe("Du läufst an der Häuserwand des Alten Marktes entlang und gelangst schließlich zum Haus deines Großvaters. Du machst dich mit Hilfe deines Taschenmessers kurz am Schloß der alten Holztüre zu schaffen, sie geht fast sofort aus. Den Trick hast du schon als Kind gemacht, wenn du mal wieder deinen Schlüssel zu Hause vergessen hattest - und er klappt immer noch wunderbar. Du hastest die Türe hinauf. Immer noch donnert es draußen, so laut, wie du es schon lange bei keinem Gewitter mehr gehört hast. Auf jeder Etage drückst du den Lichtschalter, aber keiner schaltet das Licht an. Auch hier ist vollkommener Stromausfall. Kurz bevor du die oberste Etage erreichst, hören die Sirenen auf zu heulen, was für dich nur zum Vorteil sein kann, steht doch noch eine alte motorbetriebene E57. Du obersten Treppen sind am kürzesten und am verwinkelsten. Links die Waschküche läßt du liegen, da kannst du nichs sehen. Du stürmst nach rechts, den kurzen Gang entgang und reißt die hintereste Türe auf der rechten Seite auf. Du stürmst auf das Dachfenster an der Südseite zu. Überall siehst du dunkelgraue Wolkenberge sich auftürmen. Statt bis nach Düsseldorf kannst du nicht einmal bis an die Westgrenze Mönchengladbachs sehen. Du wendest den Blick ab und läufst zu einem der Fenster an der Nordseite. Hier bietet sich die ein gleiches Bild. Die Wolken sind so dicht, daß du nicht einmal den Gladbacher Wasserturm in dieser Brühe sehen kannst. Bleiben noch die Fenster an der Südseite. Bereits als du dich ihnen näherst, erkennst du, daß du hier ein weiteres Sichtfeld haben wirst. Du reißt das Fenster auf um besser sehen zu können. Von oben peitschen dicke Regentropen herab, aber das aufgeklappte Fenster schützt dich weitestgehend. Die Wolkenwand ist hier einige Kilometer entfernt. Da plötzlich wird es hell in der Wolkenwand. Wie gebannt starrst du dahin. War das ein Blitz? Da wieder. Wieder ein Blitz. Wieder in der Wolkenwand. Das ist jetzt aber sehr ungewöhnlich. Minutenlang starrst du auf die Wolkenwand - und auch an zwei oder drei anderen Stellen erblickst du immer wieder kurze Blitze - mitten in der Wolkenwand, aber fast auf Bodenhöhe. Ein mulmiges Gefühl breitet sich in deinem Magen aus. Gerade, als du das Fenster schließen willst, kommt etwas aus der Wolke. Etwas riesiges. Ein Objekt, wie ein Turm, aber es bewegt sich, wie ein Stelzenläufer. Und ein Lichtbogen, wie ein Blitz spannt sich von ihm aus, dann brennt etwas zu seinen - Beinen? - während es sich weiter in Richtung der Gladbacher Innenstadt voranschiebt.\n Deine Nackenhärchen haben sich aufgerichtet. Du weißt zwar nicht genau, was das ist, aber es bringt Zerstörung, soviel ist sicher. Hastig schließt du das Fenster. Du rennst aus dem Dachstuhl heraus zurück in den Flur und eilst die Treppen hinab, bis du unten an den der Haustüre ankommst. Was wirst du tun?");
	tripodgesehen = true;
	auswahl("Du läufst hinaus und zu dem Polizeiwagen (1), du läufst die Kellertreppe hinab und suchst dort Schutz vor dem was kommt (2) oder du läufst zurück zu Elke, der Frau, die dich täglich mit Backwaren versorgt hat und erzählst ihr, was du gesehen hast (3)?", 3, ort8, ort20, ort7);
}

void ort10(void) {
// Kampf
	raum = 10;
	ort13();
	textausgabe("Du bist nicht stolz darauf, einen anderen Menschen getötet zu haben, aber du warst ja nicht der Angreifer. Trotzdem fühlst du dich schäbig. Etwas in dir hat sich verändert, das kannst du spüren. Noch immer prasselt der Regen auf dich, so als wäre nichts gewesen. Und hinter den Wolkenbergen, da bist du dir sicher, scheint immer noch die Sonne.");
	auswahl("Willst du jetzt weiter zum Haus deines Großvaters (1) oder zurück zum Geschäft (2)?", 2, ort9, ort7);
}

void ort11(void) {
// fliehen
	raum = 11;
	versuchedeinglueck(ort12, ort13);
	ort10();
}

void ort12(void) {
// entkommen
	raum = 12;
	textausgabe("Geduckt rennst du an der Häuserwand entlang und dann rechts in die Straße, auf das Kabuff zu, dort rennst du links um die Ecke in Richtung Kapuzinerplatz, läufst dann aber stattdessen um die Kirche herum, ein kurzes Stück über den Parkplatz - und dann die Stepgesstraße hinunter, bis du schließlich wieder die Hindenburgstraße erreichst. Etwas atemlos stellst du dich an die Wand. Mehr als nur einmal bist du beim Laufen geschlittert. Das Wasser läuft in Strömen in die Kanalisation. Du vergewisserst dich, bleibst mehrere Minuten in deiner versteckten Position, bis du dir sicher bist, daß der Angreifer dir nicht gefolgt ist. Erst jetzt begibst du dich zurück zur Bäckerei.");
	ort7();
}

void ort13(void) {
	// Begegnung mit einem Zufallsgegner der "Zivilseite"
	int zufallsgegner = wuerfel(6);
	bool kampfausgang;
	charakter_t gegner1 = { "verängstigte Frau", 2, 2, 3, 3 };
	charakter_t gegner2 = { "Plünderer", 6, 6, 3, 3 };
	charakter_t gegner3 = { "aggressiver Jugendlicher", 6, 6, 4, 4 };
	charakter_t gegner4 = { "Polizist", 5, 5, 4, 4 };
	charakter_t gegner5 = { "Gutbürger", 6, 6, 5, 5 };
	charakter_t gegner6 = { "Heckenschütze", 8, 8, 4, 4 };
	charakter_t gegner7 = { "wahnsinniger Polizist", 6, 6, 4, 4 };
	switch(zufallsgegner) {
		case 1: kampfausgang = kampf(&spieler, &gegner1, 1, false, NULL);
				break;
		case 2: kampfausgang = kampf(&spieler, &gegner2, 1, false, NULL);
				break;
		case 3: kampfausgang = kampf(&spieler, &gegner3, 1, false, NULL);
				break;
		case 4: kampfausgang = kampf(&spieler, &gegner4, 1, false, NULL);
				break;
		case 5: kampfausgang = kampf(&spieler, &gegner5, 1, false, NULL);
				break;
		case 6: kampfausgang = kampf(&spieler, &gegner6, 1, false, NULL);
				break;
		default: kampfausgang = kampf(&spieler, &gegner7, 1, false, NULL);
				break;
	}
	if(!kampfausgang)
        beenden(rot, EXIT_SUCCESS, "Das war nicht dein bester Kampf. Um ehrlich zu sein, das war dein schlechtester Kampf - und auch dein letzter Kampf. Dein allerletzter Kampf, den du nicht überlebt hast. Mit dir ist es zu ENDE gegangen.");
	getoetetemenschen += 1;
	if(wuerfel(6) >= 4) {
		if(objekt[pistole] <= 1)
			if(janeinfrage("Willst du die Pistole deines Gegners an dich nehmen (j/n)?"))
				objekt[pistole] += 1;
	}
	else {
		if(objekt[gewehr] <= 0)
			if(janeinfrage("Willst du das Gewehr deines Gegners an dich nehmen (j/n)?"))
				objekt[gewehr] += 1;
	}
	objekt[patrone] += wuerfel(8);
}

void ort14(void) {
// Klopfen und rufen
	raum = 14;
	if(wuerfel(6) >= 4) {
		textausgabe("Dein Klopfen hat dich unvorsichtig gemacht. Du hast einen unerwarteten Gast angelockt.");
		ort13();
	}
	else textausgabe("Bisher scheinen deine Bemühungen nicht zu fruchten. Drinnen reagiert Nichts und niemand auf dein Klopfen.");
	auswahl("Willst du es weiter mit klopfen und rufen probieren (1), oder willst du versuchen, ob du die Türe öffnen kannst (2), oder willst du dir einen anderen Weg suchen (3)?", 3, ort14, ort15, ort16);
}

void ort15(void) {
// Versuchen, die Türe aufzubekommen
	raum = 15;
	versuchedeinglueck(ort18, ort19);
}

void ort16(void) {
// Versuchen, einen anderen Weg zu finden
	raum = 16;
}

void ort17(void) {
// Auf dem Weg zu Elkes Auto
	raum = 17;
	textausgabe("Du schulterst dir deinen Rucksack, während Elke ein paar Papiertüten mit Backwaren füllt. Danach siehst du ihr zu, wie sie ein paar Flaschen aus dem Kühlschrank holt und in eine weitere Tüte stopft. Als sie alles gepackt hat, verschwindet sie kurz im Nebenzimmer und kommt, jetzt einen Mantel tragend, eine Damenhandtusche um die Schulter hängend zurück. Ihr nennt die ganzen Taschen und verlasst das Geschäft. Elke schließt den Laden hinter sich ab, macht sogar die Bewegung zum Scharfschalten der Alarmanlage, dann sagt sie: \"Komm!\" und geht dir voraus, die Stepgesstraße hinunter.\nMißtrauisch wirfst du einen Blick zum Jugendheim hinüber. Oft genug, haben dir da schon junge Kerle aufgelauert. Diesmal aber bleibt alles ruhig. Elke führt dich ein kurzes Stück weiter und hält vor einer Parkhaustüre. Sie holt einen Schlüssel heraus und schließt auf. Gemeinsam geht ihr in das Innere des Parkhauses. Das Treppenhaus stinkt nicht so muffig und nach Urin, wie es oft der Fall ist, wenn Betrunkene sich eine Stelle für ihre Notdurft suchen. Ihr steigt hinab bis zum dritten Untergeschoß.\nSchließlich erreicht ihr Elkes Auto, einen schon etwas älteren Kombi. Sie entriegelt das Auto per Knopf. Ihr verladet eure Taschen und deinen Rucksack im Kofferraum und setzt euch in das Auto, wobei du auf dem Beifahrersitz Platz nimmst. Das kleine Licht der Mittelkonsole ist im Moment die einzige aktive Lichtquelle im ganzen Parkhaus.\nElke fummelt am Autoradio herum, aber das Digitalradio findet keinen Sender den es darstellen kann. Schließlich gibt sie die Suche auf. \"Hier drinnen haben wir keinen Empfang, ich müßte schon aus dem Parkhaus herausfahren, damit wir etwas hören könnten.\"");
	auswahl("Stimmst du Elke zu - und ihr fahrt mit dem Auto aus dem Parkhaus heraus in den Regen (1) oder hast du ein mulmiges Gefühl und glaubst nicht, daß das eine gute Idee ist (2)?", 2, ort100, ort101);
}

void ort18(void) {
// Du bekommst die Türe auf
	raum = 18;
	textausgabe("Du betrittst den Laden, hast aber sofort das Gefühl, alleine zu sein. Du durchstöberst jedes Zimmer, jeden Raum, jeden Winkel, aber Elke ist nicht mehr da. Sie hat wohl nicht geglaubt, daß du wieder kommst. Dir bleibt nichts anderes übrig, als den Laden zu verlassen.");
	ort34();
}

void ort19(void) {
// Die Türe geht nicht auf
	raum = 19;
	textausgabe("Du bekommst es einfach nicht hin, die Türe geht nicht auf. Du denkst, daß sie wohl nicht mehr da sein wird und wendest dich ab.");
	ort34();
}

void ort20(void) {
// Im Keller des Großvaters
	raum = 20;
	textausgabe("Du öffnest die eiserne Treppe und gehst die Kellertreppe hinunter. Als du am Fuße der Treppe ankommst, siehst du vor dir die eiserne Doppeltüre, die in den Heizungskeller führt. Nach rechts führt ein weiterer Weg zum Lagerraum, wo deine Großmutter ihre Kartoffeln lagerte. Neben der Treppe führt ein Weg nach hinten, wo die Mieter des Hauses ihre Kellerabteile haben.");
	auswahl("Wenn du zurück nach oben gehen willst (1). Möchtest du in den Heizungskeller (2). Willst du in den Gang, der zum Kartoffellager deiner Großmutter führt (3). Würdest du gerne in den rückwärtigen Bereich gehen (4).", 4, ort21, ort22, ort23, ort24);
}

void ort21(void) {
// Treppenflur
	raum = 21;
	if(dreistelzer) {
		textausgabe("Der Hausflur sieht mitgenommen aus, doch die Steintreppe ist immer noch intakt. Der Zählerkasten hängt von der Wand herunter und die Treppe zur Kneipe wurde eingetreten. Dahinter siehst du das Spiel von Flammen an der Wand. Du gehst durch die Türe. Die Kneipe sieht verwüstet aus. Du gehst durch den Gang in die Küche, durchquerst sie und stehst in der ehemaligen Garage deines Großvaters, wo bequem vier Autos Platz hätten. Du erinnerst dich, wie vor vielen Jahren hier der Vogelschuß des Schützenfestes stattgefunden hatte - und schaust hinauf zu den Glasbausteinen, die einen Teil des Bodens vom Dachgarten ausgemacht haben. Der Ort, wo deine Schaukel stand, wo du Schnittlauch von dem gepflanzten Gemüse gegessen hattest, wo Clarence und Judy manchmal Auslauf haben durften. Von innen kannst du die Türe öffnen und zum Kapuzinerplatz hinaushuschen.");
		auswahl("Willst du es riskieren, die Treppe hochzusteigen (1), der Steintreppe in den Keller folgen (2) oder das Haus in Richtung Kapuzinerplatz verlassen (3)?", 3, ort25, ort20, ort28);
	}
	textausgabe("Du stehst in einem großen Treppenflur. An der Seite befindet sich der Hängeschrank mit den ganzen Ablesegeräten für Elektrizität und Wasser. Rechts an der Wand für eine Steintreppe hinauf in das erste Stockwerk. Geradeaus ist eine Holztüre, durch die man in den Kellerbereich der Gaststätte kommt, allerdings ist sie dauerhaft abgeschlossen. Rechts neben der Holztüre, unterhalb der Steintreppe, befindet sich eine Eisentüre, hinter der sich der Abstieg in den Keller befindet.");
	auswahl("Willst du die Treppe hinaufsteigen (1), in den Keller hinuntergehen (2), oder das Haus verlassen und zurück auf den Alten Markt (3)?", 3, ort25, ort20, ort26);
}

void ort22(void) {
// Heizungskeller
	raum = 22;
	if(agartha == true)
		textausgabe("Ein großteil der Kohlen liegt auf dem Boden. An der rußgeschwärtzten Wand kannst du das Wort \"Agartha\" lesen. Der Heizungskessel arbeitet derzeit nicht. Sonst befindet sich ncihts im Heizungskeller.");
	else {
		textausgabe(" Als du die Türe öffnest, ist es recht stickig im Raum. Der Heizkessel ist aus, obwohl in einem großen Verschlag an der Seite sich die Kohlen nur so stapeln. Als du genauer hinguckst, hast du das Gefühl, den geritzen Buchstaben \"A\" an der Wand zu sehen.");
		if(janeinfrage("Willst du soviele der Kohlen zur Seite räumen, um zu sehen, ob das \"A\" vielleicht Teil eines Wortes ist? Eine letzte Nachricht deines Großvaters an dich? (j/n)")) {
			textausgabe("Deine Kleidung verdreckt vollständig, während du auf den Kohleberg krabbelst - und Brikett umd Brikett wegräumst. Noch dazu machst du eine Menge Krach.");
			if(wuerfel(6) >= 4)
				ort13();
			textausgabe("Endlich hast du das Gekritzel an der Wand freigelegt. Was du liest, ist ein Wort: \"Agartha\"");
			agartha = true;
		}
	}
	auswahl("Willst du in in den Gang zum Kartoffelkeller (1), oder willst du zu den rückwärtigen Kellen (2) oder möchtest du zurück nach oben in den Treppenflur (3)?", 3, ort23, ort24, ort21);
}

void ort23(void) {
// Gang zum Kartoffelkeller
	raum = 23;
	if(agartha && !schluesselgefunden) {
		textausgabe("Du gehst durch den nach Moder riechenden Gang auf das ehemalige Kartoffellager deiner Großmutter zu. Du wendest dich nach links und stehst im Türrahmen. Da erinnerst du dich. Du siehst nach links, neben dem Rahmen war ein kurzer Nagel, an dem ein Schlüssel mit einem weißumrandeten Plastik-Schlüsselkärtchen hin. Auf dem weißen Schild war mit der krakeligen Handschrift deines Großvaters dein eigener Name niedergekritzelt.\nDer Nagel steckt zwar noch in der Hand, aber er deutet nach unten - und kein Schlüssel hängt daran. Du kniest dich nieder - und durchwühlst den mit Würmern belebten Karttoffel-Schimmel-Matsch am Boden. Der Geruch steigt dir unangenehm durch die Nase. Minutenlang durchpflügst du den Matsch. Der Verzweiflung nahe willst du schon aufgeben, als dein Finger etwas kaltes berührt. Schnell wühlst du den Gegenstand frei. Es ist der Schlüsselring - mit dem Schlüssel daran.\nZärtlich streichst du den Schmutz von dem Schlüsselkärtchen ab, doch die Handschrift deines Großvaters ist nicht mehr erkennbar. Dennoch nimmst du den Schlüssel an dich.\nDu stehst auf und verläßt den Kartoffelkeller und gehst zurück.");
		schluesselgefunden = true;
	}
	else
		textausgabe("Du gehst durch den nach Moder riechenden Gang. Als du an das ehemalige Kartoffellager deiner Großmutter kommst, mußt du feststellen, das es einfach nur leer und verdreckt ist. Seit ihrem Tod hat es wohl niemand mehr betreten. Die Kartoffeln müssen schon vor Jahren verschimmelt sein. Du wendest den Blick zur Seite. Die Klappe, durch die früher immer die Bierfässer hinabgelassen wurden, befindet sich immer noch am Ende des Ganges. Jetzt würde sie allerdings in den Wintergarten der Gaststätte führen. Da du hier nichts weiter finden kannst, beschließt du zurückzugehen.");
	ort20();
}

void ort24(void) {
// Rückwärtige Keller
	raum = 24;
	textausgabe("Du gehst an der Kellertreppe vorbei. Hinter ihr macht der Gang einen Knick nach rechts, und nach einem kurzen Durchang stehst du vor einem breiten Gang. Überall sind durch hölzerne Boxen getrennt die Kellerabteile der Mieter dieses Hauses. Der Verschlag, hinter dem deine Sachen gesichert waren, ist leer, bis auf die Erinnerungen daran in deinem Kopf. Du drehst dich um und gehst zurück.");
	ort20();
}

void ort25(void) {
// Die Treppe hinaufsteigen
	raum = 25;
	if(!buchgefunden && schluesselgefunden) {
		textausgabe("Du steigst die Treppen hinauf bis zum Dachgeschoss. Hinter der vorletzten Türe auf der rechten Seite, befindet sich das Spielzimmer, daß dein Großvater dir schenkte. Hier hatte er auf einer Spanplatte eine Eisenbahn mit Bergen und Tunneln für dich aufgebaut, auch wenn er es meistens war, der die Züge fahren ließ. Du steckst den Schlüssel, den du im Keller gefunden hast, ins Schloss - und schließt die Türe auf. Du öffnest sie - und vor dir schwingt sie nach innen. Die Eisenbahn ist immer noch fest montiert auf der Spanplatte, die auf dem rötlichen Boden liegt. Du trittst ein - und schließt die TÜre hinter dir, schließt sie sogar ab, um ungestört zu sein. Auf einem Holzstuhl findest du ein Buch. Als du es dir nimmst und umdrehst, ist es der fehlende Band aus der Bibliothek. Groß prangt der Name Agartha darauf. Du setzt dich auf den Stuhl und blätterst es durch. Immer wieder stößt du auf Passagen in dem Buch, die dein Großvater mit Bleistift unterstrichen hat. In den Passagen ist die Rede von einer Stadt, die sich in den Tiefen der Erde befinden soll. Es finden sich Referenzen darauf, das es Eingänge zu dieser Stadt geben soll - und das wenigstens einer dieser Eingänge in Tibet ist. Plötzlich flattert ein Blatt aus dem Buch. Es muß zwischen den Seiten gelegen haben. Du bückst dich und hebst es auf. Mit der krakeligen Schrift deines Großvaters steht darauf notiert: \"Münster   Sarg   Innenhof   Keller   Rheydt\"\nDu steckst den Zettel und das Buch in deinen Rucksack.");
		buchgefunden = true;
	}
	else {
		if(wuerfel(6) > 4)
			textausgabe("Du steigst die Treppen bis zur dritten Etage hinauf. Von hier aus schaust du hinüber zu dem Balkon, wo früher dein Freund Guido gelebt hat. Du erinnerst dich daran, wie ihr ein primitives Blechdosentelefon gespannt hattet, dessen eines Ende ihr mit einem Holzflitzebogen ihr hinübergeschossen hattet. In Erinnerungen versunken steigst du danach die Treppe wieder hinunter ins Erdgeschoss.");
		else
			textausgabe("Du ersteigst die Treppenstufen bis hinauf in das Dachgeschoss. Noch einmal versuchst du, ob du eine der verschlossenen Türen aufbekommst und schaust durch die Schlüssellöcher, doch erscheinen dir alle Räume leer und verlassen. Du drehst dich um und gehst die Treppenstufen wieder hinab in das Erdgeschoss.");
	}
	ort21();
}

void ort26(void) {
// Der Alte Markt
	raum = 26;
	if(dreistelzer) {
		textausgabe("Die Fassaden der Häuser am Alten Markt brennen knisternd, einige drohen einzustürzen. Tote Leiber bedecken den Ort, den ein ein Dreistelzer entweiht hat, der nun an damit beschäftigt ist, aus dem Polizeirevier einen Aschehaufen zu produzieren. Eine Art Panzerwagen steht unter ihm - und du könntest schwören, das gerade zwei Gestalten in die Bank gelaufen sind. Hoffentlich bist du bisher niemandem aufgefallen.");
		auswahl("Wenn du Glück hast, kannst du ungesehen in das Haus deines Großvaters huschen (1), du kannst aber auch versuchen, die Turmstiege ungesehen zu erreichen (2) oder versuchen, die unbemerkt an den Häusernwänden des Kapuzinerplatzes entlang zu bewegen (3)", 3, ort9, ort44, ort28);
	}
	if(tripodgesehen && (getoetetegegner > 0)) {
		textausgabe("Der Alte Markt ist in beißenden Qualm gehüllt, während zeitgleich Regen in Strömen herniederprasselt. Ein riesiger Dreistelzer steht auf der Kreuzung zur Aachener Straße und aus einer Öffnung leckt züngelt eine lange blaugrüne Flamme gegen die Fassade der naheliegenden Häuser. Zu versuchen, den Kapuzinerplatz oder die Turmstiege zu erreichen, dürfte keine gute Idee sein.");
		auswahl("Wie es aussieht, kannst es versuchen die Waldhausener Straße zu erreichen (1), der Weg den Abteiberg hinunter an der Polizeiwache vorbei sollte auch möglich sein (2). Auch der Weg den Marktstieg entlang (3) oder die Hindenburgstraße hinab (4) erscheinen dir noch sicher.", 4, ort43, ort35, ort27, ort34);
	}
	textausgabe("Du befindest dich auf dem Alten Markt. Regen gießt in Strömen herab, so daß du nur undeutlich die Silhouette der Kirche Mariä-Himmelfahrt ausmachen kannst, während der Brunnen mit der kleinen Statue davor vollkommen unsichtbar ist. Der Brunnen füllt sich langsam mit dem herniedergehenden Wasser, dennoch erinnert er dich, nur an ideenlos aufeinandergestapelte Zementbauklötze. Die Stühle der Eisdiele sind über den Markt und die Straße verteilt.");
	if(wuerfel(6) > 4) {
		textausgabe("Ein Schuß schlängt hinter dir in die Hausfassade ein.");
		ort113();
	}
	auswahl("Von hier aus kannst du zur Waldhausener Straße (1), dem Kapuzinerplatz (2), dem Marktstieg (3), den Abteiberg hinunter (4), zur Hindenburgstraße (5) oder zum Haus deines Großvaters (6)", 6, ort43, ort28, ort27, ort35, ort34, ort9);
}


void ort27(void) {
	// Der Marktstieg
	if(raum == 37)
		textausgabe("Atemlos kommst du an der Ecke Stadtmauer/Marktstieg an. Du denkst nicht, daß der Dreistelzer in der Ferne dich bemerkt hat.");
	if(wuerfel(6) > 4)
		textausgabe("Der Marktstieg ist für dich mit vielen sentimentalen Erinnerungen verbunden. An den schrängen blonden Sohn des Metzgers zum Beispiel, mit dem du dich hier öfters getroffen hattest. Einmal warst du sogar in der Schlachterei hier drin. Es war wohl einer der Auslöser, warum du nie wirklich auf Fleisch standest. An der Ecke war eine Bäckerei. Du hast sie geliebt, vor allem für die Süßigkeiten und das Eis, das du dir hier stets gekauft hast. Schade, daß die nette alte Bäckerin nicht mehr da ist.");
	raum = 27;
	if(dreistelzer)
		auswahl("Von hier aus könntest du weiter zum Kapuzinerplatz (1) oder aber, wenn du mutig genug bist, die Kaiserstraße hinab (2). Auch die Wallstraße (3) dürfte noch sicher sein.", 3, ort28, ort29, ort45);
	textausgabe("Du bewegst dich an der Hauswand entlang bis zur Ecke.");
	auswahl("Möchtest du von hier aus weiter zur Hindenburgstraße (1), zum Alten Markt (2), zum Kapuzinerplatz (3), zur Wallstraße (4) oder zur Kaiserstraße (5)?", 5, ort34, ort26, ort28, ort45, ort29);
}

void ort28(void) {
	// Der Kapuzinerplatz
	raum = 28;
	if(dreistelzer) {
		textausgabe("Der Kapuzinerplatz ist in beißenden Qualm gehüllt, während zeitgleich Regen in Strömen herniederprasselt. An der Westseite des Platzes sieht man ab und zu grüne Flammen durch die Luft züngeln. Der Dreistelzer ist weitergezogen, wobei er eine Schneise der Verwüstung hinterlassen hat. Der Platz hat sich in eine Kakophonie aus toten Leibern und geplatzten Träumen verwandelt, die sich auf den düsteren Pflastersteinen verteilen. Flammen züngeln aus den Schächten der Tiefgarage hinauf.");
		ort113();
		if(wuerfel(6) > 4)
			textausgabe("Der Kapuzinerplatz ist wohl nicht länger ein Ort des Friedens in deiner Erinnerung. Nicht der Ort des Glücks, in dem du in deiner Kindheit so manche Kirmes und Trödelmarkt erlebt hast. Dafür aber spukt dir durch den Kopf, wie du einmal Karneval mitten während der Kirmes hier überfallen wurdest.");
		auswahl("Von hier aus kannst du zum Alten Markt (1), zur Turmstiege (2), das Haus Zoar betreten (3), zur Kaiserstraße (4), zum Marktstieg (5) oder zum Haus deines Großvaters (6)", 6, ort26, ort44, ort30, ort29, ort27, ort9);
	}
	if(wuerfel(6) > 4)
		ort113();
	else
		textausgabe("Du siehst hinauf zu dem Haus, wo Guido mit seiner Mutter gewohnt hat. Vor deinem geistigen Augen siehst du für einige Momente den Krahn, um dessen Arm herum Marios und du Frisbees in einer Art Bumerang-Technik geworfen haben. Du siehst die aufgebaute Bühne vor deinem inneren Auge, die Plakette der kölschen Rockband, der Figuren ihr ausgeschnitten habt. Du stellst dir vor, wie du Bierdeckel wie Shuriken durch die Gegend wirfst, während gleichzeitig deine Kleidung immer nasser wird vom Regen.");
	if(wuerfel(6) > 3)
		textausgabe("Deine Erinnerung an den Überfall auf den Juwelier kommt dir wieder in den Sinn. Wie du das Auto gesehen hast, und die Polizei riefst - und als du dann hinuntergingst, als der Polizeiwagen da war, schickten sie dich weg - weil du ein Kind warst. Nicht mal an dem Nummernschild hatten diese Idioten Interesse.");
	auswahl("Von hier aus kannst du zum Alten Markt (1), zur Turmstiege (2), das Haus Zoar betreten (3), zur Kaiserstraße (4), zum Marktstieg (5) oder in das Haus deines Großvaters (6)", 6, ort26, ort44, ort30, ort29, ort27, ort9);
}

void ort29(void) {
	// Die Kaiserstraße
	raum = 29;
	if(wuerfel(6) > 4)
		textausgabe("Vermutlich wirst du dich niemals daran gewöhnen, daß die Autos jetzt in verkehrter Richtung die Kaiserstraße und die Regentenstraße entlang fahren.");
	if(wuerfel(6) > 4)
		textausgabe("Für einen Moment bist du abgelenkt, und wärst fast zu dem Kinocenter hinübergegangen, um dir die Bilder zu Filmen anzugucken, wie ihr als Kinder das immer auf den Kinos am Kapuzinerplatz gemacht habt. Damals gab es das Kinocenter hier nicht. Stattdessen waren hier Häuser - und ein großer Parkplatz. Auch an die alte Marienschule erinnerst du dich noch.");
	if(wuerfel(6) > 4)
		textausgabe("Am liebsten würdest du zu dem Haus gehen, wo früher der Kindergarten war, den du besuchtest. Du erinnerst dich an die Spielkameraden von damals, die Turnhalle im ersten Stock, den Hof hinter dem Haus und das fahruntüchtige Auto, das ihr dort hattet. An das Klettergerüst, das Laufen auf Stelzen - und auch an die Prügel, die du oft von anderen, die kräftiger waren als du, bezogen hast.");
	if(wuerfel(6) > 5)
		ort113();
	else
		ort13();
	textausgabe("Es scheint so, als wäre die Kaiserstraße nicht der beste Ort, um sich dort lange aufzuhalten.");
	auswahl("Möchtest du die Straße hinab zur Blücherstraße (1), zur Wallstraße (2), zum Marktstieg (3) oder zum Kapuzinerplatz (4)?", 4, ort31, ort45, ort27, ort28);
}

void ort30(void) {
	// Haus Zoar
	raum = 30;
	if(wuerfel(6) > 4)
		textausgabe("Du näherst dich dem Haus in dem du in deiner Jugend oftmals Billard gespielt hast, bis dieser Irre aus Hephata mit einer Pistole auf dich geschossen hatte. Vergessen hast du den Tag niemals, er war für dich wie eine Wiedergeburt. Du hast das Drumherum nie vergessen. Wenn du genau drüber nachdenkst, sah der Blödmann Chico tatsächlich so aus, wie einer der Zeichner der MAD-Comics.");
	if(wuerfel(6) > 4) {
		textausgabe("Die Eingangstüren hängen nur lose in den Angeln. Jemand hat sich wohl bereits heute Zutritt zu dem Haus verschafft. Du bist dir nicht sicher, ob es eine gute Idee war, sich dem Gebäude zu nähern, aber deine Erinnerungen und deine Neugierde siegt. Als du die Treppe hinuntergehst, hörst du ein Geräusch aus Richtung des Kickers. Du springst mit einem Hechtsprung von der Treppe auf das nahe Sofa.");
		ort13();
	}
	else
		textausgabe("Die Eingangstüren hängen nur lose in den Angeln. Jemand hat sich bereits Zutritt verschafft. Vorsicht gehst du in das Innere, aber alles ist ruhig. Du schleichst die Treppe herunter. Es sieht immer noch so aus wie in deiner Jugend. Der Kicker und der Billardtisch, nichts hat sich hier verändert, selbst die beiden Sofas sind noch da. In der Turnhalle brennt eine Notlampe - und ein Fenster steht weit offen.");
	auswahl("Willst du zurück auf den Kapuzinerplatz (1) oder durch das geöffnete Fenster hinabspringen und hinüber zum Beginn der Kaiserstraße laufen (2)?", 2, ort28, ort29);
}

void ort31(void) {
	// Blücherstraße
	if(raum == 51) {
		textausgabe("Du siehst dich um, aber im Moment ist alles still und du fühlst dich sicher.");
		mahlzeit();
	}
	if(raum == 29)
		textausgabe("Nachdem du die Kaiserstraße entlang gegangen bist, ist zu deiner rechten Hand der Adenauerplatz mit seinem häßlichen Sandspielplatz, während zu deiner Linken die Stadtbibliothek lockt. Rechts von dir verläuft die Kleiststraße bis hin zum Lichthof.");
	if(raum == 50)
		textausgabe("Du folgst der Kleiststraße entlang, passierst den Adenauerplatz und stehst schließlich an der Kreuzung Kaiserstraße. Dir gegenüber auf der anderen Straßenseite liegt die Stadtbibliothek.");
	if(wuerfel(6) > 4)
		textausgabe("Du erinnerst dich, das ein Stück weiter geradeaus, Nikos Mutter ihr Geschät hatte, während Niko auf der Regentenstraße wohnte. Das letzte Mal, als du etwas von ihm hörtest, war seine Stimme, aus dem Radio. Die kanntest viele Leute, die auf der Regentenstraße wohnten. Dagmar, Nicola, ihren Bruder den Nazi (den du nicht abkonntest, weil er ihr Bruder war - und weil er Nazi war) - und Thomas, der Computerfreak und Ärztehörer, Dietrich der Tänzer lebte auch dort und Dirk, der total auf Adam Ant abfuhrt. Für dich war die Regentenstraße immer der Ort, wo du deine Grundschule besuchtest, Fußball spieltest und dir ab und zu bei der Apotheke dein Junior-Heft oder eine Dose Pullmoll holtest.");
	raum = 31;
	auswahl("Willst du den Hügel hinauf die Kaiserstraße entlang (1), der Kleiststraße folgen (2) oder der Stadtbibliothek deine Aufwartung machen (3)?", 3, ort29, ort50, ort32);
}

void ort32(void) {
	// Stadtbibliothek
	if(verzeichnisgelesen && agartha && !kartegefunden) {
		textausgabe("Du erkletterst hastig den Geröllhaufen, über den du in das Innere der Stadtbibliothek gelangst. Du gehst die Reihen der Apothekerschränke ab, bis du das Fach erwischst, in der Du dir einen Hinweis auf das Wort \"Agartha\" erhoffst. Als du endlich die Karte mit dem Begriff hast, stehen darauf 3 Bücher. Du nimmst dir die Karte heraus und durchstöberst die Bibliothek. Zwanzig Minuten später, als du das dritte Buch in der Hand hältst, erkennst du unter den Ausleihern des Buches deinen Großvater in der Liste. Beim Durchblättern des Buches bemerkst du, daß darin eine Karte abgebildet ist. Du liest dir die Texte der Innenumschläge durch und staunst nicht schlecht. In dem Buch ist die Rede von einer inneren Welt. Du erinnerst dich an das Buch von Jules Verne, daß dir dein Großvater geschenkt hatte. Es liest sich unglaublich, daß auch diese Theorie Vernes wohl auf einem Körnchen Wahrheit beruht. Agartha soll einer dieser Orte sein. Das Buch verstaust du kurzerhand in deinem Rucksack, schulterst ihn und verläßt auf dem schnellsten Wege die Stadtbibliothek.");
		kartegefunden = true;
		raum = 32;
		ort31();
	}
	if(!verzeichnisgelesen || !agartha) {
		textausgabe("Die Stadtbibliothek erscheint von außen betrachtet menschenleer. Ein Teil der vorderen Fassade ist eingebrochen, du kletterst über den dadurch entstandenen Geröllhaufen und gelangst in das Innere der Bibliothek. Da es keinen Strom gibt, bist du dankbar für die Unmenge an Apothekerschränken in deren ausziehbaren Schubladen es nur so von Karteikartenbeschreibungen der Bücher wimmelt. Leider aber hast du keinerlei Idee, wonach du suchen könntest. Da du nichts in der Bibliothek findest, was dir momentan helfen könnte, verläßt du sie.");
		verzeichnisgelesen = true;
		raum = 32;
		ort31();
	}
	textausgabe("Kaum daß du die Bibliothek betreten hast, glaubst du hinter den Regalen eine Gestalt wahrzunehmen.");
	ort13();
	textausgabe("Um nicht noch in weitere Scherereien hineinzustolpern, verlä8t du die Bibliothek.");
	raum = 32;
	ort31();
}

void ort33(void) {
	// Im Lichthof
	textausgabe("Der Lichthof sieht aus wie ein Scherbenmeer. Hier muß es bereits zu heftigen Kämpfen gekommen sein. Die angrenzende Passage ist voller Rauch.");
	if(dreistelzer) {
		textausgabe("Vor dem unteren Ausgang des Lichthofs steht der Metallfuß eines Dreistelzers. Das hier ist eine Falle, eine Sackgasse. Du drehst dich um und rennst so schnell du kannst Richtung der Stadtbibliothek davon.");
		raum = 33;
		ort50();
	}
	if(verzeichnisgelesen && (wuerfel(6) > 3))
		ort113();
	if(raum == 50) {
		raum = 33;
		auswahl("Möchtest du zur Hindenburgstraße hinab laufen (1) oder lieber zurück die Kleiststraße entlang zur Kaiserstraße (2)?", 2, ort51, ort50);

	} else {
		raum = 33;
		auswahl("Willst du die Passage hinauf zur Kleistraße laufen (1) oder kehrst du lieber um und läufst die Hindenburgstraße hinauf (2) oder hinab (3)?", 3, ort50, ort34, ort51);
	}
}

void ort34(void) {
	// Hindenburgstraße, oberer Teil
	if(dreistelzer) {
		textausgabe("Von hier aus kannst du sehen, das sich am unteren Teil der Hindenburgstraße, hinter dem Knick beim Kaffeehausröster, sich ein weiterer Dreistelzer befindet. Du kannst nicht ihn selber sehen, aber ab und an sieht man den Schimmer seiner Flammenwaffe auf den Häuserfassaden und in den Fenstern wiederspiegeln. Es wäre nicht gerade klug, sich in die Richtung zu wagen. Ähnlich sieht es auch an der Stepgesstraße aus, auch von dort kommen die grauenvollen mechanischen Geräusche der Dreistelzer. Der dritte im Bunde steht immer noch neben dem Polizeirevier und verunstaltet das Gebäude.");
		raum = 34;
		auswahl("Du kannst von hier aus versuchen, die Krichelstraße zu erreichen - und es von dort aus zum Münsterplatz zu versuchen (1) oder du versuchst in die Wallstraße zu huschen (2), wenn du schnell genug bist, kannst du auch an der Stadtmauer entlang zum Marktstieg (3)", 3, ort37, ort45, ort27);
	}
	if(wuerfel(6) > 4)
		textausgabe("Für einen Moment siehst du vor deinem geistigen Auge, wie aus dem klotzigen Modegeschäft wieder das Kaufhaus wurde, in dessen Verkaufsräumen du viele Stunden deiner Kindheit verbracht hast, mit dem Betrachten von Spielzeugen, dem hineinschnuppern in Büchern und dem herumtippen auf 8-Bit Computern. Du erinnerst dich sogar wieder daran, wie an deinem Geburtstag Ephraim Kishon hier war und Autogramme in seine Bücher schrieb.");
	textausgabe("Während du die Hindenburgstraße entlang läufst, kannst du in der Ferne außer dem Nebelschleier auch einen gewissen Rauchschleier ausmachen.");
	if(wuerfel(6) > 4)
		textausgabe("Im obersten Stockwerk der Häuserzeile in dem sich auch der Burgerfresstempel befindet, steigen erste Flammen in die Höhe, die aber glücklicherweise durch den stetigen Regen in ihrer Ausbreitung behindert werden.");
	if(wuerfel(6) > 5)
		ort113();
	auswahl("Von hier aus kannst du weiter die in Richtung Lichthof (1), in Richtung des Hauptbahnhofs (2), zur Wallstraße (3), in Richtung des Alten Markts (4) oder in Richtung Abteiberg (5). Du kannst es auch die Croonsallee entlang zur Kaiserstraße (6).", 6, ort33, ort51, ort45, ort26, ort35, ort29);
}

void ort35(void) {
	// Abteiberg
	if(raum == 46) {
		textausgabe("Als du langsam den Abteiberg hinaufsteigst, erblickst du am oberen Ende des Berges drei Gestalten in seltsamen Uniformen. Sie tragen eine Art von Helmen, die an Taucherglocken erinnern, mit großen Filtern daran wie von Gasmasken. Und wie es scheint, haben sie dich gesehen. Als Fluchroute hast du jetzt nur noch den hinter dir liegenden Geroweiher.");
		charakter_t soldat[3] = { { "1. Soldat", 7, 7, 6, 6 }, { "2. Soldat", 6, 6, 4, 4}, { "3. Soldat", 7, 7, 8, 8} };
		if(!kampf(&spieler, soldat, 1, false, ort46))
            beenden(rot, EXIT_SUCCESS, "Sich auf drei kampferprobte Soldaten war einerseits sehr mutig von dir, andererseits aber auch sehr dumm. Du sinkst tödlich getroffen von einer Kugel zu Boden, während du die Schritte ihrer genagelten Stiefel näher kommen hörst. In Gedanken blickst du durch das Haus rechts von dir hindurch. Dahinter befindet sich ein Hügel, die Mauer, die zum Münstervorplatz führt, und an dieser Mauer wachsen Ranken herunter. Guido, Marco und du - ihr habt hier früher Verstecken gespielt, und du bist immer klammheimlich an den Ranken  gewesen - hast gewartet, bis sie oben an dir vorbei sind - und dann hinter ihrem Rücken, während sie die Treppen hinunterstiegen, nach oben geklettert. Warum warst du dieses Mal nicht so schlau, diesen Weg zu nehmen. Dann, als die Tritte der Soldaten auf dich einprasseln, naht für dich das ENDE.");
	}
	raum = 35;
	if(wuerfel(6) > 4)
		textausgabe("Es kommt dir immer noch befremdlich vor, daß die \"neue\" Polizeiwache jetzt an der Ecke ist. Früher in deiner Kindheit, seit ihr Kinder immer außen herum an der Statue Balderichs herumgekraxelt. Das jetzt zu versuchen, würde wahrscheinlich eine Menge Ärger mit der Bullerei mit sich bringen, obwohl ... jetzt wohl nicht mehr. Die Eingangstüren liegen zertrümmert am Boden.");
	textausgabe("Der Abteiberg führt hinab zum Geroweiher, gleichzeitig aber auch zum Rathaus und dem Münster. Man sieht die Wand aus dem Boden ragen - und fühlt sich ein wenig an eine mittelalterliche Burg erinnert, auch wenn niemals eine hier stand.");
	if(wuerfel(6) > 4)
		textausgabe("Wer hatte dir bloß die Geschichte erzählt, das früher der Urin, der Kot, und das Wasser der Waschzuber den Abteiberg hinunterlief, während unten die Leprakranken ihr krankes Dasein fristeten?");
	auswahl("Möchtest du von hier aus zum Alten Markt (1), die Hindenburgstraße hinab (2), den Vorplatz am Münster entlang (3), das alte Rathaus betreten (4)? oder zum Park am Spatzenberg (5)?", 5, ort26, ort34, ort37, ort36, ort52);
}

void ort36(void) {
	// Das Rathaus
	textausgabe("Das alte Rathaus von Mönchengladbach ist vermutlich viel weniger alt, als der Name vermuten läßt. Es hat eine große Toreinfahrt - und man kann von ihm aus in den Innenhof des Münsters gelangen, wenn du dich nicht irrst. Etwas idiotisch ist der Parpkatz davor - und die Straße die nach Links zum Abteiberg-Museum abbiegt. Nur ein Spiegel soll die Autofahrer davor schützen, das sie an dieser uneinsehbaren stelle mit ihren Autos Unfälle bauen. Viel intelligenter wäre es wohl, die Durchfahrt hier bautechnisch nicht mehr zu ermöglichen, aber wer bist du, daß dich solche Gedanken überhaupt interessieren?");
	if(wuerfel(6)>4)
		textausgabe("Beim Betreten des Rathauses erinnerst du dich, wie du für deine Fackel, die tatsächlich deine Mutter gebastelt hatte, du in das Rathaus gebeten wurdest - dir wurde für die Fackel gedankt - und dann wurde sie untem im \"Alten Zeug Haus\" am Abteiberg ausgestellt. Und das alles nur, weil \"deine\" Martinsfackel von den Wappen der Bundesrepublik Deutschland, des Landes Nordrhein-Westfalen und dem alten und neuen Wappen der Stadt Mönchengladbach geziert wurde. Heute würdest du deswegen wahrscheinlich von \"Urheberrechtsschützern\" gejagt werden.");
	if((raum == 41) && durchganggeoeffnet) {
		dreistelzer = true;
		textausgabe("Der ganze Boden erhebt, und du hörst ein lautes mechanisch Geräusch, als du oben auf dem Alten Markt einen Dreistelzer sehen kannst, der sich in Stellung bringt. Schreie ertönen aus seiner Richtung, dann siehst du zwei fremde Soldaten, die in deine Richtung die Treppe von Mariä Himmelfahrt heruntergelaufen kommen. Ein weitere stürmt aus der Polizeiwache heran, er ist ein richtiger Hühne.");
		charakter_t soldat[3] = { { "1. Soldat", 6, 6, 7, 7 }, { "2. Soldat", 5, 5, 6, 6}, { "3. Soldat", 8, 8, 7, 7} };
		if(!kampf(&spieler, soldat, 1, false, ort46))
            beenden(rot, EXIT_SUCCESS, "Sich auf drei kampferprobte Soldaten war nicht gerade deine klügste Entscheidung. Du sinkst tödlich getroffen von einer Kugel zu Boden, während du die Schritte ihrer genagelten Stiefel näher kommen hörst. In Gedanken blickst du hinüber zur Treppe, die von Mariä Himmelfahrt herunterführt. Du erinnerst dich, wie ihr als Kinder auf den Sohlen eurer Sandalen im Stehen das Geländer heruntergerutscht seid, so als wärt ihr Wellenreiter in den Brandungen vor Hawai. Dann landet der Absatz eines Stiefels in deinem Gesicht. Der Schmerz explodiert in deinem Gesicht, alles wird Schwarz. Und dann ist es auch schon vorbei. Dein ENDE ist gekommen.");
		raum = 36;
		auswahl("Du bist dir nicht sicher, ob es eine gute Idee ist, zum Alten Markt zu laufen. Immerhin bleibt dir noch der Rückzug in den Innenhof des Münsters (1), du könntest auch den Abteiberg hinunterlaufen, der Hügel sieht zu steil für den Dreistelzer aus (2), oder den Vorplatz des Münsters entlanglaufen (3) in der Hoffnung, es vielleicht zum Geroweiher zu schaffen. Zu guter letzt bleibt dir noch die Flucht durch die Gasse, in der Hoffnung, die Hindenburgstraße zu erreichen (4)", 4, ort41, ort35, ort37, ort51);
	}
	raum = 36;
	auswahl("Möchtest du hinaus auf den Abteiberg (1) oder den Innenhof betreten (2)?", 2, ort35, ort41);
}

void ort37(void) {
	// Der Vorplatz des Münsters
	if(raum == 52) {
		textausgabe("So schnell dich deine Beine tragen, rennst du am Waffengeschäft vorbei, vorbei an den Häusern, in denen Stefan und Dirk lebten (der diesen geilen TI 99/4a hatte) - und die Straße hinunter. Du greifst das Schild und schleuderst um die Ecke der Weiherstraße. Das \"Alte Zeug Haus\" läßt du rechts liegen, der \"Alte Ulan\" ist auch nicht dein Ziel, sondern die Treppenstufen, die zum Münsterplatz hochführen. Du nimmst sie so schnell wie möglich und fällst fast noch über die letzte Stufe, dann bist du wieder oben. Und vollkommen außer Atem. Du ringst nach Luft und betrachtest das stets geschlossene Portal des Münsters auf dieser Seite.");
		raum = 37;
		auswahl("Du kannst vo hier aus jetzt nach links und zum Rathaus gehen - wobei da natürlich die Gefahr ist, vom Dreistelzer an der Polizeiwache bemerkt zu werden (1), du kannst auch das Münster betreten (2), sinnloserweise die Treppen zum Geroweiher hinunterspurten (3) oder den Abteiberg entlang zum oberen Teil der Hindenburgstraße spurten (4)", 4, ort36, ort38, ort46, ort34);
	}
	if(raum == 34) {
		textausgabe("Du spurtest das kurze Stück \"An der Stadtmauer\" entlang und biegst in die Krichelstraße ab, wo du direkt in einen Soldaten rennst.");
		ort113();
		textausgabe("Kaum daß du den Kampf gewonnen hast, läuft du weiter. Kurz bevor du den Kirchplatz erreichst, läufst du nach links, den Weg entlang, das kurze Stück über die Brücke hinüber zum Museum, dort die Treppe hinunter und um die Probstei herum, bis du schließlich vor dem Haupteingang des Münsters stehst.");
		raum = 37;
		auswahl("Willst du in das Münster hinein (1), oder an ihm vorbei zum Rathaus laufen, in der Gefahr, auf den Dreistelzer zu stoßen (2), oder willst du die Treppenstufen hinab zum Park am Geroweiher (3)?", 3, ort38, ort36, ort46);
	}
	if(raum == 51) {
		textausgabe("Als du an oben am Hügel ankommst, rennst du am Elektronikladen links herum. Du nimmst die Treppe, hinauf zum Museum und rennst dort über die Steinplatten, auf denen du in deiner Kindheit mit deinen Freunden Skateboard gefahren und Roller Skater gelaufen bist, aber diesmal heißt der Sport nicht Bremstest (wobei ab und an das Skateboard unter dem Gitter hindurchschoß), sondern überleben, einfach nur überleben. Du rennst über das Museum und dann die Treppe hinab, ein kurzes Stück durch den Park und dann hinüber, um das Münster herum. Etwas erschöpft stehst du nun vor dem Haupteingang. Dein Blick schweift über den Geroweiher. Bis jetzt siehst du dort unten keine Dreistelzer, allerdings machen der Wind und der Regen es nicht leicht, viel zu erkennen.");
		raum = 37;
		auswahl("Willst du in das Münster hinein (1), oder an ihm vorbei zum Rathaus laufen, in der Gefahr, auf den Dreistelzer zu stoßen (2), oder willst du die Treppenstufen hinab zum Park am Geroweiher (3)?", 3, ort38, ort36, ort46);
	}
	if((raum == 46) && dreistelzer) {
		textausgabe("Statt offen die lange Treppe am Münster hochzulaufen, bist du das kurze Stück durch den Park rechts gespurtet, in dem du noch am Morgen dein Zelt aufgebaut hattest. Dann bist du seitlich hinten am Kirchenschiff herum gelaufen. Nun überlegst du, wie es weiter gehen soll.");
		raum = 37;
		auswahl("Du kannst in das Münster hinein (1), oder du könntest vesuchen, am ehemaligen Quelle vorbei und dann schnell über die Hindenburgstraße zu huschen, in der Hoffnung nicht bemerkt zu werden - und den Marktstieg erreichen (2)", 2, ort38, ort27);
	}
	raum = 37;
	auswahl("Willst du weiter in Richtung des Abteibergs (1), die Treppen hinab zum Geroweiher (2) oder willst du in das Gladbacher Münster hinein (3)?", 3, ort35, ort46, ort38);
}

void ort38(void) {
	// Im Hauptschiff des Münsters
	if(dreistelzer) {
		textausgabe("Jetzt wo die Dreistelzer da draußen lauern, bist du dir nicht sicher, ob dieser uralte Bau dich noch lange schützen wird. Doch für einen Augenblick nimmst du dir Zeit. Die Zeit darüber nachzudenken, warum zum Teufel diese Typen da draußen sind - und deine Geburtsstadt in Schutt und Asche legen. Zu einer Erklärung gelangst du aber leider nicht.");
		mahlzeit();
		auswahl("Willst du das Münster verlassen (1) oder willst du in den Seitengang (2)?", 2, ort37, ort39);
	}
	else
		textausgabe("Das Hauptschiff der Kirche ist erst vor kurzem neu restauriert worden und erstrahlt in hellstem weiß. Die Kirche wird trotz ihrer Größe weniger bedrückend als so manch alter Göttertempel. Der Duft des Weihwassers erfüllt die Luft. Ein Ständer beherbergt Bücher und Pamphlete, während an anderer Stelle, Kerzen aller Größe brennen. Es ist ein seltsamer Ritus in fast allen Religionen, sinnlose Brandrituale durchzuführen, die der Umwelt nicht wirklich helfen - dafür aber die Ressourcen dieses einzigen Planeten, auf dem wir leben angreifen. Du könntest dich auf einer der Bänke niederlassen, dennoch fühlst du dich hier nicht wohl. Seitlich an der Wand steht eine Türe offen. In deiner Kindheit war eine der Mieterinnen deines Großvaters verantwortlich für die Schatzkammer, sie war öfters mit dir in diesem Gang, der unter anderem zu jener Kammer führte.");
	mahlzeit();
	auswahl("Du kannst das Münster verlassen (1) oder den Seitengang betreten (2).", 2, ort37, ort39);
}

void ort39(void) {
	// Im Seitengang des Münsters
	textausgabe("Der Gang spannt sich um den Innenhof des Münsters. Glas gibt an jeder Stelle den Blick darauf frei, ebenso wie auf die Steinumtafelte Treppe, die in der Mitte des Rasenhofs nach unten führt. Auf der rechten Seite führt eine kurze Treppe hinab in die Gruft unter dem Hauptschiff.");
	auswahl("Möchtest du in das Hauptschiff des Münsters (1), den Innenhof betreten (2) oder die Treppe zur Gruft hinuntersteigen (3)?", 3, ort38, ort41, ort40);
}

void ort40(void) {
	// Die Gruft unter dem Altar
	textausgabe("Du steigst die Treppe hinunter in die Gruft, die direkt unter dem Altar des Münsters liegt. Du erinnerst dich, wie in der dritten Klasse einmal ein Klassenausflug hierhin führte und die Person, die euch das Münster zeigte, auch an diesem Ort mit euch kam. Sie erzählte eine krude Sage, von Soldaten, einem Spiel und dem Abdruck der Hand des Teufels auf dem Sarg noch einmal genauer an.");
	if(durchganggeoeffnet && !sargverschoben) {
		textausgabe("Eigentlich hattest du erwartet, den Sargdeckel geöffnet vorzufinden, aber alle drei Särge sind geschlossen.");
		if(janeinfrage("Möchtest du den Sargdeckel erneut verschieben (j/n)?"))
			sargverschoben = true;
		textausgabe("Du wendest dich ab und gehst die Treppe hinauf.");
		raum = 40;
		ort39();
	}
	if(kartegefunden && !sargverschoben)
		if(janeinfrage("Willst du dir den Inhalt des Sarges näher ansehen (j/n)?")) {
			textausgabe("Mit aller Kraft gelingt es dir, wenn auch nur sehr langsam, den Sargdeckel zu verschieben - allerdings nicht sehr weit. Doch was du bereits sehen kannst, reicht dir aus: der Sarg ist vollkommen leer. Du blickst dich noch kurz um, und überlegst, ob du auch die Deckel der anderen Särge verschieben sollst. Angesichts des unnützen Kraftaufwands und der Tatsache, das bereits in diesem Sarg nichts drin ist, bist du der festen Überzeugung, das es sich bei den anderen ebenso verhält. Du wendest dich ab und gehst die Treppe hinauf.");
			sargverschoben = true;
			raum = 40;
			ort39();
		}
	if(sargverschoben) {
		if(janeinfrage("Willst du den Sargdeckel wieder in seine ursprüngliche Position schieben (j/n)?")) {
			textausgabe("Mit aller Kraft gelingt es dir, wenn auch nur sehr langsam, den Sargdeckel wieder zu schließen. Als du es endlich geschafft hast, könntest du schwören, ein dumpfes, leises Geräusch von irgendwoher vernommen zu haben.");
			sargverschoben = false;
		}
	}
	textausgabe("Da die Gruft ansonsten leer ist, verläßt du sie und steigst die Treppe hinauf.");
	raum = 40;
	ort39();
}

void ort41(void) {
	// Der Innenhof
	auswahl("Durch eine Türe gelangst du in das Münster (1), ein weiterer Durchgang führt in das alte Rathaus (2). Eine Treppe in der Mitte des Innenhofes führt zu einer Stahltüre hinab (3).", 3, ort39, ort36, ort42);
}

void ort42(void) {
	// Der Keller unter dem Innenhof
	if(sargverschoben) {
		textausgabe("Du steigst die Stufen in der Mitte des Innenhofes hinab und öffnest die Stahltüre. Als du in den Raum hineinblickst, erkennst du, das sich ein Spalt in einer Wand geöffnet hat. Du gehst in den Raum hiein und bemerkst beim näherkommen, das der Spalt tatsächlich eher ein richtiger Durchgang ist - und das dahinter ein Gang liegt.");
		raum = 42;
		auswahl("Möchtest du den Gang betreten (1) oder willst du den Raum verlassen und die Treppe hinauf in den Innenhof gehen (2)?", 2, ort53, ort41);
	}
	textausgabe("Du steigst die Stufen in der Mitte des Innenhofes hinab und öffnest die Plastikklinge an der Stahltüre. Die Türe öffnet sich problemlos. Der Innenraum riecht muffig, die Wände sind alt und nicht überall eben. Der Raum wird offensichtlich für nichts mehr benutzt. Da es hier nichts weiter zu sehen gibt, drehst du dich um, verläßt den Raum und steigst die Treppe hinauf in den Innenhof.");
	raum = 42;
	ort41();
}

void ort43(void) {
	// Die Waldhausener Straße
	raum = 43;
	textausgabe("Die Waldhausener Straße war früher das Herzstück der Mönchengladbacher Altstadt. Hier reihten sich die Kneipen und Diskotheken nur so aneinander, doch in den Anfangszeit der 1990er Jahre, hatten die christdemokratischen Hohlbirnen der Stadt dem ein Ende bereitet - und damit nachhaltig dem Flair der Stadt geschadet. Vor deinem geistigen Auge stellst du dir das ehemalige Blumengeschäfft der Schallenburgers vor. Du erinnerst dich daran, wie deine Mutter und ihr Mann oftmals in den Herbstmonaten dort Kränze gebunden hatten. Und daran, wie sie von den Schallenburgers die alte Nähmaschine bekamen, in der 10.000 Mark versteckt waren. Glücklicherweise waren sie so ehrlich, Micky und seiner Mutter das Geld zurückzugeben. Trotzdem wurde Micky nicht alt, und die Schallenburgers und die Geschichte ihres Blumen- und Friedhofsgeschäftes endeten bald darauf.");
	if(dreistelzer) {
		textausgabe("Zurück in der Gegenwart jedoch mußt du erkennen, das in weiter Entfernung, die Waldhausener Straße entlang in Richtung Hardt, wenigstens 2 Dreistelzer sich an den Häusern und Menschen der Stadt vergehen - während ein weiteres dieser Ungetüme nicht weit bergauf steht.");
		auswahl("Am sichersten wäre es wohl, den Fliescherberg hinabzulaufen (1), eventuell wäre auch der Schleichweg die Turmstiege entlang eine Alternative (2)", 2, ort52, ort44);
	}
	auswahl("Die Straße führt hinauf zum Alten Markt (1), neben dem Dicken Turm verläuft die Turmstiege (2) und ein weiterer Weg führt durch den kleinen Grünbereich des Fliescherberges (3)", 3, ort26, ort44, ort52);
}

void ort44(void) {
	// Die Turmstiege
	textausgabe("Die Turmstiege ist herrlich geschützt. An ihrem Ende zur Waldhausener Straße hin führt eine Treppe neben dem Dicken Turm hinab, von ihr selber eine Stiege den Turm hinauf und die lange Mauer ermöglicht es, dank der hervorragenden Steine an ihr hinaufzuklettern. Das taten deine Freunde und du schon als Kinder - und das tust du auch jetzt noch manchmal, den oben in der kleinen Zinne sieht einen Niemand.");
	mahlzeit();
	auswahl("Von hier aus hast du die Möglichkeit zur Waldhausener Straße zu gelangen (1), durch die Passage zum Alten Markt zu gehen (2) oder zum Kapuzinerplatz(3), wenn du schnell genug spurtest schaffst du es vielleicht sogar bis zum Haus deines Großvaters (4)", 4, ort43, ort26, ort28, ort9);
}

void ort45(void) {
	// Die Wallstraße
	raum = 45;
	if(dreistelzer) {
		textausgabe("Von hier aus kann man sehen, das aus Richtung des Wasserturms sich ein wenigstens 30 Meter hoher Dreibeiner dem Krankenhaus nähert.");
	}
	if(wuerfel(6) > 4)
		textausgabe("Deine Gedanken schweifen ab zu Marios, der früher über dem Cafe wohnte. Oben in seiner Wohnung hattet ihr Commodore Basiclistings in den PC eingehämmert und stundenlang dann mit den Ergebnissen gespielt. Von hier aus seid ihr auch oft mit Dimmi zum Fußballspielen aufgebrochen. In der Bäckerei wohnte Michaela mit ihren Eltern, die die Backstube betrieben. Du hast leider nie erfahren, warum sie nach Ende der dritten Klasse nicht in die Schule zurückkehrte - oder wieso plötzlich die Bäckerei weg war.");
	textausgabe("Du hast die Wallstraße immer gerne als Abkürzung benutzt, um zur Hindenburgstraße zu gelangen, nicht zuletzt wegen des Bücherladens, an dem du dir täglich am Schaufenster die Nase platt gedrückt hast, wo du dir das erste mal Michael Moorcocks \"Elric von Melnibone - Die Sage vom Ende der Zeit\" holtest, jenes Buch, das du dir sechs Mal kaufen mußtest, weil niemand es zurückgab, wenn man es ihm auch nur einmal auslieh. Und immer nach seinem Neuerwerb, hast du es nochmal gelesen. Du erinnerst dich auch noch an deine schräge Klassenkameradin, die einerseits total schüchtern her war vom Wesen - und die dennoch wie Boy George herumlief - und auch die Musik die ganze Zeit über hörte.");
	auswahl("Du kannst von hier aus zur Hindenburgstraße (1), die Kaiserstraße hinab (2), den Marktstieg entlang (3) oder am Haus Zoar vorbei zum Kapuzinerplatz (4)", 4, ort34, ort29, ort27, ort28);
}

void ort46(void) {
	// Der Geroweiher
	if(raum == 55) {
		textausgabe("Mit lange Zügen tauchst du weiter durch das eiskalte Wasser. Deine Lungenflügel leeren sich und beginnen langsam zu brennen. Du weißt, daß du den Rückweg nicht mehr schaffen würdest, also schwimmst du was das Zeug hält. Plötzlich bemerkst du über dir ein Licht - du hältst drauf zu - und durchbrichst die Wasseroberfläche. Luft strömt in deine gequälten Lungenflügel. Du ruderst mit den Armen schaffst es, nicht wieder einzutauchen und ruderst halb benommen zum Ufer, an dem du liegenbleibst. Du mußt etwas verschnaufen. Dann, als du wieder klarer denken kannst, nimmst du deine Umgebung wahr - und erkennst, das du am Ufer des Geroweihers liegst, unter den schützenden Ästen der alten Trauerweide, an denen ihr als Kind immer mit Tarzanschrei ins Wasser geschwunden seid. Du bleibst weiter einfach sitzen und schwelgst in Erinnerungen, bis du dir die aktuelle Gefahr wieder vor Augen führst und dich erhebst.");
	}
	else
		textausgabe("Der Geroweiher.\nEin kleiner Ort der Ruhe, an dem ein Spielplatz in deiner Jugend für ein gewisses Training deiner Muskeln sorgte, der aber auch der Austragungsort so mancher Keilerei war. Im Zentrum steht ein Stück alter Stadtmauer und wenn man hochguckt, sieht man auf dem Hügel das Münster. Der Weiher ist vielleicht gerade einmal zwei, höchstens drei Meter weit. Wenn du dich recht entsinnst, hatte eure Grundschullehrerin euch früher erzählt, der Geroweiher würde durch den Gladbach gespeist werden. Aber egal wie weit du auch zurückdenkst, den Gladbach hast du hier noch nirgendwo fließen sehen. Vielleicht, ist er ja nur noch eine Erinnerung, an alte Zeiten.");
	if(dreistelzer) {
		textausgabe("Dank des Regens wäre es dir fast nicht aufgefallen, das zwei Dreistelzer sich nahe der Kreuzung zur Aachener Straße und in Gegenrichtung nahe der Rheydter Straße platziert haben. Zwar gibt es noch den kurzen Tunnel hinüber zur Turmstraße, aber wer weiß, wie es sonst in dem Viertel aussieht? In Anbetracht der Situation - und bei dem Gedanken daran, das noch ein weiterer Dreistelzer oben am Markt lauert, wären die beiden sicheren Routen, um von hier weg zu kommen, wohl nur der Weg den Spatzenberg entlang, oder der Weg hinauf zum Münster. Die Abhänge sind deine Verbündeten. Du kannst dir nicht vorstellen, daß diese 30 Meter hohen mechanischen Ungetüme dort entlangstelzen können.");
		raum = 46;
		auswahl("Willst du also zum Spatzenberg (1) oder zum Münster (2) hinauf?", 2, ort52, ort37);
	}
	raum = 46;
	auswahl("Du kannst den Spatzenberg hinauf (1), oder den Abteiberg (2) oder die Treppen zum Münstervorplatz nehmen (3)", 3, ort52, ort35, ort37);
}

void ort47(void) {
	// Die Bismarkstraße
	raum = 47;
	if(dreistelzer) {
		textausgabe("Von der Ecke der Hindenburgstraße aus kannst du sehen, das zwei Dreistelzer sich an den Bankgebäuden zu schaffen machen. Soldaten bringen aus der Bank heraus Säcke zu einem Panzerwagen. Für deinen Geschmack ist das etwas zu nah, um auch nur im Entferntesten daran zu denken, die Kreuzung zu überqueren. So drehst du dich lieber um und suchst dein Heil in der Flucht, die Hindenburg straße hinauf.");
		ort51();
	}
	else
		textausgabe("Ein Dreibeiner steht neben der Kaiser-Friedrich-Halle und bedeckt ihr dach mit einem blau-grünlichen Flamme. Glücklicherweise ist er damit soweit weg, daß wohl kaum die Gefahr besteht, bemerkt zu werden.");
	auswahl("Der Weg ist soweit sicher die Hindenburgstraße hinauf (1) oder weiter hinab bis zum Vorplatz des Hauptbahnhofs (2)", 2, ort51, ort48);
}

void ort48(void) {
	// Der Bahnhofsvorplatz
	raum = 48;
	if(dreistelzer) {
		textausgabe("Von überall her, dringen die scheusslichen, mechanischen Geräusche der Dreistelzer auf dich ein, trotzdem ist hier auf dem Europaplatz noch keines dieser haushohen Metallmonster aufgetaucht.");
	}
	if(wuerfel(6) > 3)
		textausgabe("Für die meisten Gladbacher ist das hier einfach der Platz vor dem Hauptbahnhof, für andere der Busbahnhof aber kaum jemand kennt den Namen des Platzes. Früher war da mal die Bank, wo der Mann deiner Mutter arbeitete. Amüsiert mußt du daran denken, wie er fassungslos nach Hause kam und davon erzählte, das die nette Frau aus dem Haus, die manchmal den Bankern etwas zu essen brachte, die Frau war, die ihren Mann zerstückelt und in Dosen im Bunten Garten verstreut hatte.");
	textausgabe("Es stehen keinerlei Busse im Busbahnhof herum, auch sind kaum Menschen hier auszumachen. Betrachtet man nur den Platz, so wirkt alles so, wie es sein soll.");
	auswahl("Du kannst von hier aus entweder die Hindenburgstraße entlang in Richtung Alter Markt gehen (1) oder das Innere des Hauptbahnhofs betreten (2)", 2, ort47, ort49);
}

void ort49(void) {
	// Der Hauptbahnhof
	raum = 49;
	if(!tripodgesehen)
		tripodgesehen = true;
	if(wuerfel(6) > 3)
		textausgabe("Während du auf die Doppeltüren des Bahnhofs zuschreitest kommt dir ein Bild aus deiner Erinnerung. Du wolltest dir das neueste Lustige Taschenbuch holen - und es war Sonntag. Normalerweise kam es erst Montags heraus, so wie auch das Yps-Heft, aber am Kiosk des Hauptbahnhofs gab es alles einen Tag früher. Und so fragtest du damals im Kiosk nach, aber dort sagte man, daß die Hefte noch nicht geliefert wurden - das es aber in der nächsten Stunde geschehen würde. Und so ging ich an die Modelleisenbahn, die dort stand - und spielte damit für die nächste Stunde. Und als ich mich schließlich wieder erinnerte warum ich gekommen war, holte mir die Verkäuferin das frisch gedruckte Taschenbuch aus dem Karton heraus. Ich pflückte mir noch das neue Yps mit seinem Gimmick aus dem Verkaufsständer, bezahlte und machte mich freudig auf den Nachhauseweg. Herr Jansen der Wirt war bei meinen Eltern, daran erinnerte ich mich, er hatte mir das Geld für das Taschenbuch gegeben.");
	textausgabe("Beim Öffnen der Türe entfaltet sich ein grauenhafter Anblick vor mir, die Bahnhofshalle gleicht eher einem Schlachthaus und riecht strenger als eine Leichenhalle. Und am Ende des Bahnhofs sehe ich die metallenen Beine eines Dreistelzers. Mir bleibt nichts anderes übrig, als mich umzudrehen und das Weite zu suchen.");
	ort48();
}

void ort50(void) {
	// Die Kleiststraße
	if(raum == 33)
		textausgabe("Nervös siehst du hinter dich, aber es scheint nicht, als wäre dir jemand gefolgt.");
	else
		textausgabe("Die Kleiststraße ist jene kurze Straße zwischen dem Lichthof und dem Adenauerplatz, an dem sich das von allen Gladbachern gehasste Finanzamt befindet. Wenn man sich die Fassade des Gebäudes genauer betrachtet, so hat sie noch so etwas widerliches an sich, eine Ausstrahlung, als wäre sie ein verirrtes Stück 1940.");
	raum = 50;
	if(dreistelzer) {
		textausgabe("Ein Schuß erklingt!");
		ort113();
	}
	auswahl("Du kannst am Adenauerplatz vorbei zur Blücherstraße (1) oder Richtung Hindenburgstraße in den Lichthof (2)", 2, ort31, ort33);
}

void ort51(void) {
	// Mittelteil der Hindenburgstraße
	if(raum == 36) {
		textausgabe("Du stürzt am Jugendheim vorbei, läßt das Museum rechts an dir vorbeigleiten. Früher hättest du wohl angehalten - und dir die Schaufenster des Elektronikladens angeguckt, jetzt aber hastest du quer über die Straße und läufst bergab auf die Stepgesstraße zu. Unten am Fuß der Stepgesstraße steht ein weiterer Dreistelzer, sein eines Standbein hat ein Auto plattgequetscht. Wer immer auch in dem Auto drin saß, er dürfte nun klein wie eine Briefmarke sein. Du läuftst nach links in Richtung Croonsallee. Als du an die Kreuzung zur Hindenburgstraße kommst, hältst du an. Auch hier sieht es nicht besser aus. Der Dreibeiner auf dem Alten Markt ist auch von hier aus zu sehen - er bleckt bläuliches Feuer gegen das ehemalige Heinemann. Und trotz des prasselnden Regens erkennst du auch am unten an der Hindenburgstraße, direkt um die Biegung herum noch ein weiterer Dreistelzer sein muß. Du mußt nicht lange überlegen. Du rennst zurück, hinauf zum Rathaus und dem Münster. Im Moment scheinen daß die letzten beiden Orte zu sein, wo die Dreistelzer noch nicht hingelangen. Verbissen rennst du los.");
		raum = 51;
		ort37();
	}
	if((raum == 47) && dreistelzer) {
		textausgabe("Mit Mühe und Not gelingt es dir, die Bismarkstraße unbemerkt zu überqueren. Du hältst dich dicht an die Häuserfassade gedrückt, damit dich der andere Dreibeiner nicht bemerkt, der am Lichthof gegenüber der Friedrichstraße sein Zerstörungswerk fortsetzt. Belustigt denkst du an die Kuh Martina zurück. Zurück in der Realität rennst du so schnell dich deine Beine tragen in die Albertusstraße hinein, vorbei an dem Gebäude früher der Buchclub war, in dem deine Mutter sich seit einem halben Jahrhundert Mitgliedschaft durchgequält hat, vorbei an der Bank... Uff, das hätte auch schlecht ausgehen können. Auch hier ist eine Bank, aber noch kein Dreistelzer. Du läuftst weiter bis zum Adenauerplatz und rennst quer über diesen, bis du an die Ecke zur Blücherstraße kommst.");
		raum = 51;
		ort31();
	}
	raum = 51;
	auswahl("Von hier aus kannst du der Hindenburgstraße bergauf folgen (1) oder in die Gegenrichtung auf den Hauptbahnhof zu (2), oder Richtung Kleiststraße durch den Lichthof (3)", 3, ort34, ort47, ort33);
}

void ort52(void) {
	// Der Fliescherberg
	if(dreistelzer) {
		textausgabe("Oben von der Annastiege aus, kannst du den Dreibeiner sehen - und auch die Männer, die allem Anschein nach die Bank leer räumen.");
		raum = 52;
		auswahl("Von hier aus kannst du versuchen die Neustraße langzuhuschen, ein kurzes Stück die Weiherstraße hinab zulaufen und dann die Treppen hinauf zum Münsterplatz (1), oder du rennst hinüber zur Waldhausener Straße, zum Dicken Turm (2) oder jagst wie in deiner Jugend die gesamte Anna-Schiller-Stiege bis zum Geroweiher hinab (3)", 3, ort37, ort43, ort46);
	}
	if(wuerfel(6) > 4)
		textausgabe("Der Fliescherberg ist ein ganz sentimentaler Ort für dich. Oben auf der kleine Plattform naher der Ecke Neustraße habt ihr immer Detektiv gespielt - und die Bank beobachtet. Hier wuchsen auch die Knallerbsen mit denen ihr die Leute erschreckt habt. An den Ästen der beiden Bäume neben der Plattofrm seid ihr immer hinaufgeklettert - und wenn ihr Glück hattet, dann konnte man im Winter an ein oder zwei Tagen auch mit dem Schlitten den ganzen Fliescherberg hinuntersausen.");
	textausgabe("An der Seite des Fliescherberges führt die lange Anna-Schiller-Stiege hinab, neben der ihr als Kinder immer in den Büschen Cowboys und Indianer oder Ritter gespielt hattet. Hier waren immer eure selbst gemachten Pfeilbögen und Schwerter versteckt. Ein lang gewundener Weg führt von der Waldhausener Straße hinüber und ein weiterer von der Ecke Neustraße hinunter. Du weißt gar nicht mehr, wie oft ihr als Kinder wagemutig auf euren Skateboards hier heruntergerast seid, ja, gerast trifft es, denn der Berg ist so steil, das die Endgeschwindigkeit einfach nur verboten schnell war.");
	raum = 52;
	auswahl("Du kannst von hier aus zum Abteiberg hinaufgehen (1), hinab zum Geroweiher (2) oder hinüber zur Waldhausener Straße (3)", 3, ort35, ort46, ort43);
}

void ort53(void) {
	// Der Eingang zum Labyrinth
	if(raum == 42)
		textausgabe("Du betrittst den Gang. Er ist alt, die Luft riecht modrig, abgestanden. Aus deinem Rucksack hast du deine Taschenlampe und deinen alten Marschkompass aus der Bundeswehrzeit hervorgeholt. Du leuchtest die Wände ab und gehst langsam voran. Plötzlich hörst du hinter dir, wie sich der Spalt schließt. Du drehst dich noch um - aber es ist zu spät. Es scheint, als wäre dir der Rückweg versperrt. Hilflos zuckst du mit den Schultern. Wenn du richtig liegst, dann kannte dein Großvater diesen Ort - und er hätte dich niemals in eine Falle laufen lassen. So schreitest du weiter den Gang voran.");
	else
		textausgabe("Ganz im Westen ist der Gang eine Sackgasse. Du weißt, daß dort eine TÜre ist, aber sie ist zu perfekt eingefasst - und du findest nichts, um sie zu öffnen. So gibst du nach einer Weile des Suchens auf und folgst dem Gang nach Osten.");
	sargverschoben = false;
	ort54();
}

void ort54(void) {
	// Der Raum mit der versteckten Rutschbahn
	if(objekt[rucksack] > 0)
		textausgabe("Du betrittst einen mehr oder weniger rechteckigen Raum. Es sind verschiedene, unleserliche Kritzeleien und Schriftzeichen an den Wänden zu sehen, jedoch nichts, was du wirklich entziffern könntest.");
	else {
		textausgabe("Du tastest dich durch den stockfinsteren Raum, bis du glaubst den Durchgang nach Norden gefunden zu haben und folgst diesem weiter.");
		ort55();
	}
	raum = 54;
	if(durchganggeoeffnet && (objekt[rucksack] > 0)) {
		textausgabe("In der Südostecke des Raumes hat sich ein Stück des Bodens verschoben. Dort ist ein Loch im Boden, in dem eine Rutsche ist. Nachdem du sie dir näher angesehen und betastet hast, kommst du zu dem Ergebnis, daß das Metall zu glatt ist - um im Falle eines Falles dort wieder hochklettern zu können - hingegen wäre es wohl eine Leichtigkeit - hinunterzurutschen.");
		auswahl("Es führen zwei Wege aus dem Raum heraus, der eine führt nach Westen (1), der andere nach Norden (2). Du könntest einen von ihnen nehmen - oder aber eine ungewisse Rutschpartie wagen (3).", 3, ort53, ort55, ort56);
	}
	else
		auswahl("Es führen zwei Wege aus dem Raum heraus, der eine führt nach Westen (1), der andere nach Norden (2). Welchen möchtest du einschlagen?", 2, ort53, ort55);
}

void ort55(void) {
	// Der Durchgang zum Geroweiher
	raum = 55;
	if(!objekt[rucksack]) {
		textausgabe("Du tastest dich durch den Raum, bis du schließlich auf deinen Rucksack stößt.");
		if(janeinfrage("Möchtest du dich wieder ankleiden und deinen Rucksack, deine Taschenlampe und alles andere an dich nehmen (j/n)?"))
			objekt[rucksack] = 1;
	}
	else {
		textausgabe("Der Raum, in den du gelangst, gleicht eher einer Höhle mit Natursteinen. Es gibt nur einen Weg hinaus, der im Süden, durch den du hineingekommen bist. An seiner Westwand ist ein großes, dunkles Wasserloch.");
		if(janeinfrage("Möchtest du deine Kleidung und deinen Rucksack samt Taschenlampe ablegen und in das dunkle Wasserloch tauchen (j/n)?")) {
			objekt[rucksack] = 0;
			textausgabe("Vorsichtig steckst du einen Zeh in das Wasser. Es ist eiskalt. Langsam, ganz langsam steigst du immer tiefer rein. Der Boden sinkt soweit ab, das du ab einer gewissen Grenze nicht mehr stehen kannst. Es ist jetzt stockfinster im Raum. Schräg in der Tiefe glaubst du ein Licht wahrzunehmen. Du holst noch einmal tief Luft - und tauchst tief in das Wasser ein. Du tauchst durch eine Art natürlicher Tunnel, dessen Wände sich allerdings glatt anfühlen. An einer Stelle des Tunnels stößt du gegen einen Widerstand, aber der verflüchtigt sich sofort. Vermutlich war es nur ein verirrter Fisch.");
			durchganggeoeffnet = true;
			ort46();
		}
	}
	textausgabe("Da der Raum außer dem Wasserloch nichts zu bieten hat, verläßt du ihn und gehst zurück nach Süden.");
	ort54();
}

void ort56(void) {
	// Die Rutschbahn
	if(raum == 54) {
		textausgabe("Du nimmst deinen Rucksack vom Rücken und ziehst ihn verkehrt herum an, so daß er jetzt vor deinem Bauch ist. Das Seil an deiner Taschenlampe schlingst du fest um deine Hand, damit du sie auf keinen Fall verlieren kannst. Du hast dir die Richtung gemerkt, in der die Rutschaprtie beginnen wird. Schließlich setzt du dich auf den Rand und läßt deine Beine in diese seltsam glatte Metallröhre baumeln. Du atmest tief durch und denkst an Karl Kochs, deinen Großvater. Er hätte dich doch bestimmt niemals dazu verlockt, dich in eine Gefahr zu begeben, oder? Andererseits, wenn du an das Draußen denkst, an diesen Überfall fremder Truppen auf Mönchengladbach, an die Greuel, die sich deinen Augen heute bereits geboten haben, so kannst du froh sein, überhaupt noch am Leben zu sein. Dein Leben war so lange nur ein Kampf, auch noch den nächsten Tag erleben zu dürfen, was sollte dich von einem echten Abenteuer abhalten? Im schlimmsten Fall, wird das jetzt eine ungewöhnliche Todesart, eine Rutschpartie in den Tod.\nNoch einmal holst du Luft, gerade so, als müßtest du tauchen, dann stößt du dich ab. Du rutschst, langsam nur. Vermutlich könntest du jetzt noch mit deinen Sohlen abbremsen. Aber schon jetzt stürst du, mit hinaufklettern, wir das nichts. Und dann plötzlich, wird es steiler, abschüssiger. Du breitest die Arme aus, bildest mit deinem langgezogenen Körper und deinen Elbogen eine halbwegs stabile Haltung. Du wagst es gar nicht mehr, zu versuchen mit Händen oder Schuhsohlen die Röhre zu berühren. Ja, die Röhre, denn es ist eine Röhre, eine Röhre, die vollkommen glatt ist - und dann spürst du, das du dich in einer langen Spirale bewegst, während es immer tiefer geht - und du wirst schneller in der Spirale, sie wird weiter.\nDachtest du vor Beginn deiner Rutschpartie noch, es wäre nach 2, 3, 4 Sekunden vorbei, so weißt du jetzt, daß du besser in Minuten hättest rechnen sollen. Und dann kommt es zu einer Art Korkenziehereffekt, du verlierst die Übersicht, wo oben und unten ist, dann wird es Spiralförmig - und die Spirale flacht immer weiter ab - es kommt zu einem letzten auf und ab - und dann plötzlich trittst du aus einer Röhre aus und fliegst in einem weiten Bogen - und landest in einem relativ flachen Winkel mitten im Wasser. Jetzt bemerkst du, daß das mit dem Rucksack nicht die wirklich tolle Idee war. Du ruderst - und schaffst es, seichteres Gewässer zu erreichen - und als du mit den Knien den Boden fühlst, hörst du auf zu paddeln und stellst dich auf, dann watest du die letzten Schritte an das Ufer und setzt dich hin.\nEs war nicht wirklich schwierig, diese ganze Rutschpartie durchzustehen, aber noch dreht sich dir die Umgebung ein wenig. Du legst dich zurück und schließt die Augen.");
		// Durch den Schlaf erholt sich der Spieler vollständig
		gewandheitssteigerung(12,0);
		staerkesteigerung(24,0);
		glueckssteigerung(12,0);
		textausgabe("Du weißt nicht, wieviel Zeit vergangen ist, aber als du endlich wach wirst, fühlst du dich direkt viel wohler und auch viel ausgeruhter. Deine Kleidung ist nicht mehr nass. Und dein Rucksack liegt neben dir auf dem Boden. Langsam steigt es dir ins Bewußtsein, daß du sehen kannst. Du blickst dich um uns bemerkst, das die seltsamen Wurzeln - oder sind es Pflanzen - die sich an den Wänden langziehen, eine Art Lumineszenz ausstrahlen, in einem Spektrum, die die Höhle in eine Art angenehmes gelbliches Licht tauchen.");
	}
	raum = 56;
	rotation++;
	textausgabe("Du befindest dich in einer Art Naturhöhle mit einem großen Teich. Das Wasser ist kristallklar. Die weit entfernte Nordwand ist spiegelglatt - nicht jene natürliche Glätte eines gebrochenen Felsens, sondern künstliche Glätte die Rückschlüsse auf eine Bearbeitung läßt. Mitten in dieser befindet sich ein beinahe mannsgroßes Loch. Das ist der Ausgang der Röhre, durch die du hierhin gerutscht bist. Die Höhle ist bewachsen mit einer seltsamen Pflanze oder Wurzel, die luminiszierend ist und ein angenehmes gelbliches Licht ausstrahlt. Das Ufer ist nicht sandig, wie du es von Stränden kennst, dafür liegen dort viele Kieselsteine. Soweit du sonst erkennen kannst, ist die Höhle leer. Im Süden führt ein Weg aus ihr hinaus. Da es hier nichts gibt, was dich hält, drehst du dich um und folgst dem Weg hinaus aus der Höhle.");
	ort59();
}

void ort57(void) {
	rotation++;
	if(raum == 58) {
		raum = 57;
		textausgabe("Du hast schon das Gefühl, der Gang würde niemals enden. Dann aber scheint er doch an sein Ende zu kommen und macht einen Knick nach links.");
		auswahl("Möchtest du dem Gang nach links folgen (1) oder zurückgehen (2)? Du kannst natürlich auch nach Geheimgängen suchen (3)", 3, ort131, ort58, ort63);
	} else if(raum == 63) {
		raum = 57;
		textausgabe("Der Geheimgang, den du gefunden hast, ist verwirrend. Er ändert immer wieder sporadisch die Richtung, mal mußt du kriechen, dann wieder über Hindernisse klettern. Einmal wenigstens hast du sogar Angst, stecken zu bleiben, doch schließlich gelangst du an sein Ende. In einer schattigen Mulde trittst du aus ihm hervor.");
		auswahl("Der Gang führt hier weiter geradeaus (1), du kannst aber auch einem Weg nach rechts folgen (2) oder kehrt machen und wieder zurück an deinen Ausgangsort durch den Geheimgang, so du ihn denn wiederfindest (3).", 3, ort58, ort131, ort63);
	} else { // raum == 131
		raum = 57;
		textausgabe("Der Gang macht an dieser Stelle einen Knick nach rechts.");
		auswahl("Möchtest du dem Gang nach rechts folgen (1) oder zurückgehen (2)? Du kannst natürlich auch nach Geheimgängen suchen (3)", 3, ort58, ort131, ort63);
	}
	raum = 57;
	auswahl("Du kannst dem Tunnel nach Osten folgen (1) oder nach Süden (2) oder die Wände nach Geheimgängen absuchen (3)", 3, ort58, ort131, ort63);
}

void ort58(void) {
	rotation++;
	if(raum == 57) {
		textausgabe("Du kommst, nachdem du einem langen Tunnel gefolgt bist, an eine Abbiegung.");
		raum = 58;
		auswahl("Von hier aus kannst du weiter geradeaus gehen (1) oder der Abbiegung nach rechts folgen (2). Du kannst dich aber natürlich auch umdrehen und zurückgehen (3) oder nach Geheimgängen suchen (4)", 4, ort59, ort64, ort57, ort212);
	} else if(raum == 59) {
		raum = 58;
		auswahl("Von hier aus kannst du weiter geradeaus gehen (1) oder der Abbiegung nach links folgen (2). Du kannst dich aber natürlich auch umdrehen und zurückgehen (3) oder nach Geheimgängen suchen (4)", 4, ort57, ort64, ort59, ort212);

	} else { // raum == 64
		textausgabe("Der Gang kommt mündet hier in einen anderen Gang, der von rechts nach links verläuft.");
		raum = 58;
		auswahl("Möchtest du nach rechts gehen (1) oder nach links (2) oder drehst du dich um und gehst zurück (3)? Wenn du magst, kannst du auch nach Geheimwänden suchen (4)", 4, ort59, ort57, ort64, ort212);
	}
}

void ort59(void) {
	rotation++;
	if(raum == 56) {
		raum = 59;
		textausgabe("Du folgst dem Weg. Er wird nach kurzer Zeit zu einem behauenen Gang, der in einen Tunnel mündet. Zu deinem Bedauern mußt du feststellen, daß dein Kompass hier verrückt spielt.");
		auswahl("Willst du dem Tunnel nach links folgen (1) oder nach rechts (2) oder umdrehen und zurückgehen (3)? Wenn du es für möglich hältst, das hier ein Geheimgang ist, kannst du auch nach diesem suchen (4)", 4, ort60, ort58, ort56, ort212);		
	} else if(raum == 58) {
		raum = 59;
		textausgabe("Du gelangst an eine Abbiegung.");
		auswahl("Möchtest du dem Weg weiter geradeaus folgen (1) oder nach links gehen (2) oder dich umdrehen und zurückgehen (3)? Du kannst gerne auch nach Geheimgängen suchen (4)", 4, ort60, ort56, ort58, ort212);
	} else { // raum == 60
		raum = 59;
		textausgabe("Du gelangst an eine Abbiegung.");
		auswahl("Möchtest du dem Weg weiter geradeaus folgen (1) oder nach rechts gehen (2) oder dich umdrehen und zurückgehen (3)? Du kannst gerne auch nach Geheimgängen suchen (4)", 4, ort58, ort56, ort60, ort212);
	}
}

void ort60(void) {
	rotation++;
	textausgabe("Der Gang, dem du folgst, scheint endlos lang zu sein.");
	if(raum == 59) {
		raum = 60;
		auswahl("Du kannst entweder weiter dem Gang folgen (1) oder dich umdrehen und zurückgehen (2). Wenn du magst, kannst du auch die Wände nach Geheimgängen absuchen (3)", 3, ort61, ort59, ort212);
	} else { // raum == 61
		raum = 60;
		auswahl("Du kannst entweder weiter dem Gang folgen (1) oder dich umdrehen und zurückgehen (2). Wenn du magst, kannst du auch die Wände nach Geheimgängen absuchen (3)", 3, ort59, ort61, ort212);
	}
}

void ort61(void) {
	rotation++;
	textausgabe("Der Gang, dem du folgst, scheint endlos lang zu sein.");
	if(raum == 60) {
		raum = 61;
		if(wuerfel(6) > 3)
			auswahl("Du kannst entweder weiter dem Gang folgen (1) oder dich umdrehen und zurückgehen (2). Wenn du magst, kannst du auch die Wände nach Geheimgängen absuchen (3)", 3, ort61, ort60, ort212);
		else
			auswahl("Du kannst entweder weiter dem Gang folgen (1) oder dich umdrehen und zurückgehen (2). Wenn du magst, kannst du auch die Wände nach Geheimgängen absuchen (3)", 3, ort62, ort60, ort212);
	} else { // raum == 62
		raum = 61;
		if(wuerfel(6) > 3)
			auswahl("Du kannst entweder weiter dem Gang folgen (1) oder dich umdrehen und zurückgehen (2). Wenn du magst, kannst du auch die Wände nach Geheimgängen absuchen (3)", 3, ort61, ort62, ort212);
		else
			auswahl("Du kannst entweder weiter dem Gang folgen (1) oder dich umdrehen und zurückgehen (2). Wenn du magst, kannst du auch die Wände nach Geheimgängen absuchen (3)", 3, ort60, ort62, ort212);
	}
}

void ort62(void) {
	rotation++;
	if(raum == 60) {
		raum = 62;
		if(wuerfel(6) > 3) {
			textausgabe("Der Gang, dem du folgst, scheint endlos lang zu sein. Als du es fast schon nicht mehr für möglich hältst, gelangst du dennoch endlich an sein scheinbares Ende, er macht einen Knick nach rechts.");
			auswahl("Du kannst entweder weiter dem Gang folgen (1) oder dich umdrehen und zurückgehen (2). Wenn du magst, kannst du auch die Wände nach Geheimgängen absuchen (3)", 3, ort135, ort61, ort212);
		}
		else {
			textausgabe("Der Gang, dem du folgst, scheint endlos lang zu sein.");
			auswahl("Du kannst entweder weiter dem Gang folgen (1) oder dich umdrehen und zurückgehen (2). Wenn du magst, kannst du auch die Wände nach Geheimgängen absuchen (3)", 3, ort62, ort61, ort212);
		}
	} else { // raum == 133
		raum = 62;
		textausgabe("Der Gang macht hier einen Knick nach links.");
		auswahl("Du kannst entweder weiter dem Gang folgen (1) oder dich umdrehen und zurückgehen (2). Wenn du magst, kannst du auch die Wände nach Geheimgängen absuchen (3)", 3, ort61, ort135, ort212);
	}
}

void ort63(void) {
	rotation++;
	if(raum == 57) {
		raum = 63;
		textausgabe("In den Schatten der Wand findest du einen Geheimgang. Du folgst ihm, doch schon nach einiger Zeit mußt du kriechen. Er windet sich - und mit der Zeit verlierst du die Orientierung, ja du bist dir nicht einmal sicher, ob du zwischendurch vielleicht nicht auf Abzweigungen gestoßen bist. Doch endlich, nach einer schier endlos langen Zeit, vielen Windungen, Kriechgängen und Hindernissen, die du in der Dunkelheit überwinden mußtest, hast du das Ende erreichst. Du schältst dich aus den Schatten und bist in einem erleuchteten Raum.");
		auswahl("Du kannst dem Tunnel folgen (1) oder dich umdrehen und versuchen den Rückweg durch den Geheimgang zu finden (2)", 2, ort136, ort57);
	} else {
		raum = 63;
		textausgabe("Das war's dann wohl. Der Tunnel endet hier vor einer groben Felswand, in der es vor scharfen Kanten und tiefen Schatten nur so zu wimmeln scheint.");
		auswahl("Du kannst den Tunnel zurückgehen (1) oder die Wände hier nach Geheimgängen absuchen (2)", 2, ort136, ort57);
	}
}

void ort64(void) {
	rotation++;
	if(raum == 58) {
		raum = 64;
		auswahl("Du kannst weiter geradeaus gehen (1) oder nach links abbiegen (2) oder dich umdrehen und zurückgehen (3). Wenn du magst, kannst du aber auch nach Geheimgängen suchen (4).", 4, ort139, ort65, ort58, ort212);	
	} else if(raum == 65) {
		raum = 64;
		auswahl("Du kommst an eine Abzweigung, von wo aus du nur nach rechts gehen kannst (1) oder nach links (2). Natürlich kannst du auch kehrt machen (3) oder versuchen, ob du einen Geheimgang finden kannst (4)", 4, ort58, ort139, ort65, ort212);	

	} else {
		raum = 64;
		auswahl("Du kannst weiter geradeaus gehen (1) oder nach rechts abbiegen (2) oder dich umdrehen und zurückgehen (3). Wenn du magst, kannst du aber auch nach Geheimgängen suchen (4).", 4, ort58, ort65, ort139, ort212);	
	}
}

void ort65(void) {
	rotation++;
	if(raum == 64) {
		raum = 65;
		textausgabe("Du gelangst an eine Abzweigung.");
		auswahl("Möchtest du weiter geradeaus gehen (1) oder der Abzweigung nach rechts folgen (2) oder dich umdrehen und zurückgehen (3)? Du kannst auch, wenn du magst, nach Geheimgängen suchen (4)", 4, ort133, ort66, ort64, ort212);
	} else if(raum == 66) {
		raum = 65;
		textausgabe("Der Gang mündet an dieser Stelle in einen Tunnel der von links nach rechts führt.");
		auswahl("Möchtest du nach links gehen (1) oder nach rechts (2) oder dich umdrehen und zurückgehen (3)? Du kannst auch, wenn du magst, nach Geheimgängen suchen (4)", 4, ort64, ort133, ort66, ort212);
	} else { // raum == 133
		raum = 65;
		textausgabe("Du gelangst an eine Abzweigung.");
		auswahl("Möchtest du weiter geradeaus gehen (1) oder der Abzweigung nach links folgen (2) oder dich umdrehen und zurückgehen (3)? Du kannst auch, wenn du magst, nach Geheimgängen suchen (4)", 4, ort64, ort66, ort133, ort212);
	}
}

void ort66(void) {
	rotation++;
	if((raum == 65) || (raum == 133))
		textausgabe("Du gelangst an eine Abzweigung im Tunnel. Zu deiner großen Verblüffung funktioniert dein Kompass hier wieder.");
	raum = 66;
	char *text = "Du kannst dem Tunnel nach Norden (1) oder nach Süden folgen (2). Du kannst aber auch die Abzweigung nach Osten nehmen (3)";
	switch(rotation % 8) {
		case 1: auswahl(text, 3, ort65, ort74, ort140);
				break;
		case 2: auswahl(text, 3, ort65, ort73, ort140);
				break;
		case 3: auswahl(text, 3, ort65, ort72, ort140);
				break;
		case 4: auswahl(text, 3, ort65, ort71, ort140);
				break;
		case 5: auswahl(text, 3, ort65, ort70, ort140);
				break;
		case 6: auswahl(text, 3, ort65, ort69, ort140);
				break;
		case 7: auswahl(text, 3, ort65, ort68, ort140);
				break;
		default: auswahl(text, 3, ort65, ort67, ort140);
				break;
	}
}

void ort67(void) {
	rotation++;
	textausgabe("Du befindest dich in einem behauenen Raum, der Ausgänge in verschiedenfarbige Tunnel bietet.");
	if(raum == 67)
		textausgabe("Plötzlich verspürst du, wie der Raum sich bewegt.");
	raum = 67;
	switch(rotation % 8) {
		case 1: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort74, ort68, ort81, ort67);
				 break;
		case 2: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort74, ort68, ort82, ort67);
				 break;
		case 3: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort74, ort68, ort67);
				 break;
		case 4: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3), den bläulich erleuchteten Ausgang (4) oder stehenbleiben und warten (5)", 5, ort74, ort68, ort87, ort77, ort67);
				 break;
		case 5: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort74, ort68, ort88, ort67);
				 break;
		case 6: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort74, ort68, ort80, ort67);
				 break;
		case 7: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort74, ort68, ort67);
				 break;
		default: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort74, ort68, ort66, ort67);
				 break;
	}
}

void ort68(void) {
	rotation++;
	textausgabe("Du befindest dich in einem behauenen Raum, der Ausgänge in verschiedenfarbige Tunnel bietet.");
	if(raum == 68)
		textausgabe("Plötzlich verspürst du, wie der Raum sich bewegt.");
	raum = 68;
	switch(rotation % 8) {
		case 1: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort67, ort69, ort81, ort68);
				 break;
		case 2: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort67, ort69, ort82, ort68);
				 break;
		case 3: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort67, ort69, ort68);
				 break;
		case 4: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3), den bläulich erleuchteten Ausgang (4) oder stehenbleiben und warten (5)", 5, ort67, ort69, ort87, ort77, ort68);
				 break;
		case 5: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort67, ort69, ort88, ort68);
				 break;
		case 6: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort67, ort69, ort80, ort68);
				 break;
		case 7: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort67, ort69, ort68);
				 break;
		default: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort67, ort69, ort66, ort68);
				 break;
	}
}

void ort69(void) {
	rotation++;
	textausgabe("Du befindest dich in einem behauenen Raum, der Ausgänge in verschiedenfarbige Tunnel bietet.");
	if(raum == 69)
		textausgabe("Plötzlich verspürst du, wie der Raum sich bewegt.");
	raum = 69;
	switch(rotation % 8) {
		case 1: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort68, ort70, ort81, ort69);
				 break;
		case 2: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort68, ort70, ort82, ort69);
				 break;
		case 3: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort68, ort70, ort69);
				 break;
		case 4: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3), den bläulich erleuchteten Ausgang (4) oder stehenbleiben und warten (5)", 5, ort68, ort70, ort87, ort77, ort69);
				 break;
		case 5: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort68, ort70, ort88, ort69);
				 break;
		case 6: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort68, ort70, ort80, ort69);
				 break;
		case 7: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort68, ort70, ort69);
				 break;
		default: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort68, ort70, ort66, ort69);
				 break;
	}
}

void ort70(void) {
	rotation++;
	textausgabe("Du befindest dich in einem behauenen Raum, der Ausgänge in verschiedenfarbige Tunnel bietet.");
	if(raum == 70)
		textausgabe("Plötzlich verspürst du, wie der Raum sich bewegt.");
	raum = 70;
	switch(rotation % 8) {
		case 1: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort69, ort71, ort81, ort70);
				 break;
		case 2: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort69, ort71, ort82, ort70);
				 break;
		case 3: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort69, ort71, ort70);
				 break;
		case 4: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3), den bläulich erleuchteten Ausgang (4) oder stehenbleiben und warten (5)", 5, ort69, ort71, ort87, ort77, ort70);
				 break;
		case 5: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort69, ort71, ort88, ort70);
				 break;
		case 6: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort69, ort71, ort80, ort70);
				 break;
		case 7: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort69, ort71, ort70);
				 break;
		default: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort69, ort71, ort66, ort70);
				 break;
	}
}

void ort71(void) {
	rotation++;
	textausgabe("Du befindest dich in einem behauenen Raum, der Ausgänge in verschiedenfarbige Tunnel bietet.");
	if(raum == 71)
		textausgabe("Plötzlich verspürst du, wie der Raum sich bewegt.");
	raum = 71;
	switch(rotation % 8) {
		case 1: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort70, ort72, ort81, ort71);
				 break;
		case 2: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort70, ort72, ort82, ort71);
				 break;
		case 3: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort70, ort72, ort71);
				 break;
		case 4: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3), den bläulich erleuchteten Ausgang (4) oder stehenbleiben und warten (5)", 5, ort70, ort72, ort87, ort77, ort71);
				 break;
		case 5: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort70, ort72, ort88, ort71);
				 break;
		case 6: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort70, ort72, ort80, ort71);
				 break;
		case 7: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort70, ort72, ort71);
				 break;
		default: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort70, ort72, ort66, ort71);
				 break;
	}
}

void ort72(void) {
	rotation++;
	textausgabe("Du befindest dich in einem behauenen Raum, der Ausgänge in verschiedenfarbige Tunnel bietet.");
	if(raum == 72)
		textausgabe("Plötzlich verspürst du, wie der Raum sich bewegt.");
	raum = 72;
	switch(rotation % 8) {
		case 1: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort71, ort73, ort81, ort72);
				 break;
		case 2: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort71, ort73, ort82, ort72);
				 break;
		case 3: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort71, ort73, ort72);
				 break;
		case 4: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3), den bläulich erleuchteten Ausgang (4) oder stehenbleiben und warten (5)", 5, ort71, ort73, ort87, ort77, ort72);
				 break;
		case 5: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort71, ort73, ort88, ort72);
				 break;
		case 6: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort71, ort73, ort80, ort72);
				 break;
		case 7: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort71, ort73, ort72);
				 break;
		default: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort71, ort73, ort66, ort72);
				 break;
	}
}

void ort73(void) {
	rotation++;
	textausgabe("Du befindest dich in einem behauenen Raum, der Ausgänge in verschiedenfarbige Tunnel bietet.");
	if(raum == 73)
		textausgabe("Plötzlich verspürst du, wie der Raum sich bewegt.");
	raum = 73;
	switch(rotation % 8) {
		case 1: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort72, ort74, ort81, ort73);
				 break;
		case 2: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort72, ort74, ort82, ort73);
				 break;
		case 3: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort72, ort74, ort73);
				 break;
		case 4: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3), den bläulich erleuchteten Ausgang (4) oder stehenbleiben und warten (5)", 5, ort72, ort74, ort87, ort77, ort73);
				 break;
		case 5: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort72, ort74, ort88, ort73);
				 break;
		case 6: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort72, ort74, ort80, ort73);
				 break;
		case 7: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort72, ort74, ort73);
				 break;
		default: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort72, ort74, ort66, ort73);
				 break;
	}
}

void ort74(void) {
	rotation++;
	if(raum == 95)
		textausgabe("Du entdeckst im Schatten hinter einem scharfkantigen Vorsprung einen für das Auge beinahe unsichtbaren Tunnel. Du mußt dich auf Hände und Knie hinablassen und kriechst hinein. So gut es dir möglich ist, kriechst du durch einen stockdunklen Gang. Du bist dir nicht einmal sicher, in welche Richtung du krabbelst. Mehr als nur einmal hast du das Gefühl, mehr als einen Weg zur Auswahl zu haben, doch du kriechst intuitiv der Nase nach, bis du endlich, den Tunnel verlassen kannst.");
	textausgabe("Du befindest dich in einem behauenen Raum, der Ausgänge in verschiedenfarbige Tunnel bietet.");
	if(raum == 74)
		textausgabe("Plötzlich verspürst du, wie der Raum sich bewegt.");
	raum = 74;
	switch(rotation % 8) {
		case 1: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort73, ort67, ort81, ort74);
				 break;
		case 2: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort73, ort67, ort82, ort74);
				 break;
		case 3: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort73, ort67, ort74);
				 break;
		case 4: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3), den bläulich erleuchteten Ausgang (4) oder stehenbleiben und warten (5)", 5, ort73, ort67, ort87, ort77, ort74);
				 break;
		case 5: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort73, ort67, ort88, ort74);
				 break;
		case 6: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort73, ort67, ort80, ort74);
				 break;
		case 7: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem orange erleuchteten Ausgang oder stehenbleiben und warten (4)", 3, ort73, ort67, (wuerfel(6) > 3) ? ort95 : ort98, ort74);
				 break;
		default: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2), dem gelblich erleuchteten Ausgang (3) oder stehenbleiben und warten (4)", 4, ort73, ort67, ort66, ort74);
				 break;
	}
}

void ort75(void) {
	rotation++;
	textausgabe("Du befindest dich in einem behauenen Raum, der Ausgänge in verschiedenfarbige Tunnel bietet.");
	raum = 75;
	switch(rotation % 4) {
		case 1: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort76, ort78, ort75);
				break;
		case 2: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort76, ort78, ort75);
				break;
		case 3: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort76, ort78, ort75);
				break;
		default: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder dem gelblich erleuchteten Ausgang (3)", 3, ort76, ort78, ort79);
				break;
	}
}

void ort76(void) {
	rotation++;
	textausgabe("Du befindest dich in einem behauenen Raum, der Ausgänge in verschiedenfarbige Tunnel bietet.");
	raum = 76;
	switch(rotation % 4) {
		case 1: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort75, ort77, ort76);
				break;
		case 2: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort75, ort77, ort76);
				break;
		default: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort75, ort77, ort76);
				break;
		case 3: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder dem gelblich erleuchteten Ausgang (3)", 3, ort75, ort77, ort79);
				break;
	}
}

void ort77(void) {
	rotation++;
	textausgabe("Du befindest dich in einem behauenen Raum, der Ausgänge in verschiedenfarbige Tunnel bietet.");
	raum = 77;
	switch(rotation % 4) {
		case 3: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort78, ort76, ort77);
				break;
		default: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort78, ort76, ort77);
				break;
		case 1: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort78, ort76, ort77);
				break;
		case 2: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder dem gelblich erleuchteten Ausgang (3)", 3, ort78, ort76, ort79);
				break;
	}
}

void ort78(void) {
	rotation++;
	textausgabe("Du befindest dich in einem behauenen Raum, der Ausgänge in verschiedenfarbige Tunnel bietet.");
	raum = 78;
	switch(rotation % 4) {
		default: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort77, ort75, ort78);
				break;
		case 1: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort77, ort75, ort78);
				break;
		case 2: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder stehenbleiben und warten (3)", 3, ort77, ort75, ort78);
				break;
		case 3: auswahl("Du kannst dem grünlich erleuchteten Tunnel folgen (1), dem rötlich erleuchteten Tunnel (2) oder dem gelblich erleuchteten Ausgang (3)", 3, ort77, ort75, ort79);
				break;
	}
}

void ort79(void) {
	rotation++;
	raum = 79;
	textausgabe("Der Raum in dem du dich befindest hat nur einen Ausgang, der im Norden liegt. Er besteht ganz aus Rosenmarmor, doch ist seine Oberfläche überall seltsam aufgeraut und macht es schwierig, eine Stelle genauer zu betrachten. In der Mitte des Raumes befindet sich ein großer Sockel, in dessen Mitte ein Becken mit kristallklarem Wasser ist.");
	if(!schluessel66)
		if(janeinfrage("Willst du deine Hände in das Becken tauchen (j/n)?")) {
			schluessel66 = true;
			textausgabe("Du läßt die Hände in das Becken gleiten. Das Wasser fühlt sich seltsam warm an, so daß es beinahe deine Sinne betäubt. Du gleitest mit deinen Händen über die Wände des Beckens und plötzlich fühlst du, daß da ein Gegenstand ist. Du tastest danach und bekommst ihn zu greifen. Als du die Hände aus dem Becken herausholst, hälst du deinen Kristallschlüssel in der Hand, der vollkommen durchsichtig ist. Die Zinken scheinen die Zahl 66 zu formen.");
		}
	char *text = "Willst du den Raum wieder verlassen (1)? Du kannst auch nach Geheimtüren suchen (2) oder warten (3)";
	switch(rotation % 4) {
		case 1: auswahl(text, 3, ort78, ort152, ort79);
				break;
		case 2: auswahl(text, 3, ort77, ort152, ort79);
				break;
		case 3: auswahl(text, 3, ort76, ort152, ort79);
				break;
		default: auswahl(text, 3, ort75, ort152, ort79);
				break;
	}
}

void ort80(void) {
	rotation++;
	if(raum == 143)
		textausgabe("Kaum daß du den Wasserfall durchschritten hast, bemerkst du eine Veränderung deiner Kompaßnadel. Sie hört auf mit ihrem Tanz und zeigt wieder konstant nach Norden.");
	raum = 80;
	if(!schluessel99) {
		textausgabe("Du siehst etwas funkelndes im Wasser.");
		if(janeinfrage("Möchtest du im Wasser nach dem funkelnden etwas tauchen (j/n)?")) {
			schluessel99 = true;
			textausgabe("Es ist ein kupferfarbener Schlüssel, dessen Zinken so geformt sind, als würden sie die Zahl 99 ergeben.");
		}
	}
	textausgabe("Der Raum, in dem du dich jetzt befindet, scheint eine große natürliche Höhle zu sein. Sie ist bewachsen von exotischen Pflanzen. Grün ist hier eine weniger dominierende Farbe, als es sie an der Erdoberfläche zu sein scheint. Der Raum ist viel stickiger, als die Tunnel, durch die du dich seit einiger Zeit bewegst. Im nördlichen Teil der Höhle befindet sich ein großer Teich, in den ein Wasserfall hineinstürzt.");
	char *text = "Willst du in den Teich hineinwaten und dich in die herabfallenden Wasser des Wasserfalls stellen (1) oder die Höhle durch ihren Ausgang im Osten verlassen (2)?";
	switch(rotation % 8) {
		case 1: auswahl(text, 2, ort143, ort72);
				break;
		case 2: auswahl(text, 2, ort143, ort71);
				break;
		case 3: auswahl(text, 2, ort143, ort70);
				break;
		case 4: auswahl(text, 2, ort143, ort69);
				break;
		case 5: auswahl(text, 2, ort143, ort68);
				break;
		case 6: auswahl(text, 2, ort143, ort67);
				break;
		case 7: auswahl(text, 2, ort143, ort74);
				break;
		default: auswahl(text, 2, ort143, ort73);
				break;
	}
}

void ort81(void) {
	rotation++;
	raum = 81;
	char *text = "Willst du den Tunnel nach Westen nehmen (1) oder den nach Südosten(2)?";
	char *raetseltext = "Verfertigt ist's vor langer Zeit,\ndoch mehrenteils gemacht erst heut;\nsehr schätzbar ist es seinem Herrn,\nund dennoch hütet's niemand gern.";
	char *antworttext = "Bett";
	textausgabe("Der Raum, in dem du dich jetzt befindest, ist aus einem einzigen großen Smaragd heraus geschnitten. An einer Wand ist ein Symbol sichtbar.");
	weiter();
    vordergrundfarbe(rot);
	textausgabe("*###*");
	textausgabe("    #");
	textausgabe("    #");
	textausgabe("^###*");
	textausgabe("#");
	textausgabe("#");
	textausgabe("*##n_");
	vordergrundfarbe(weiss);
    if(!raetsel1) {
		textausgabe("Eine Stimme ertönt plötzlich und stellt dir eine Frage:");
		if(janeinfrage("Glaubst du, du kannst mein Rätsel lösen (j/n)?"))
			if(raetsel(raetseltext, antworttext)) {
				raetsel1 = true;
				textausgabe("Deine Antwort war ... weise!");
			} else
				textausgabe("Deine Antwort war ... unwissend!");
	}
	switch(rotation % 8) {
		case 1: auswahl(text, 2, ort67, ort82);
				break;
		case 2: auswahl(text, 2, ort74, ort82);
				break;
		case 3: auswahl(text, 2, ort73, ort82);
				break;
		case 4: auswahl(text, 2, ort72, ort82);
				break;
		case 5: auswahl(text, 2, ort71, ort82);
				break;
		case 6: auswahl(text, 2, ort70, ort82);
				break;
		case 7: auswahl(text, 2, ort69, ort82);
				break;
		default: auswahl(text, 2, ort68, ort82);
				break;
	}
}

void ort82(void) {
	rotation++;
	raum = 82;
	char *text = "Du kannst den Tunnel nach Westen (1), nach Nordwesten (2), nach Nordosten (3), nach Osten (4), nach Südwesten (5) oder nach Südosten (6)";
	textausgabe("Du befindest dich im Zentrum eines malvenfarbenen Raumes, der aus einem großen, dir unbekannten Stein herausgeschnitzt worden zu sein scheint. Sechs Türen befinden sich in den sechs Wänden des Raumes, je eine im Nordostern, im Osten, im Südosten, im Südwesten, im Westen und im Nordwesten.");
	if(!schluessel125 && raetsel1 && raetsel2 && raetsel3 & raetsel4 && raetsel5) {
		textausgabe("In der Mitte des Raumes schraubt sich langsam ein Sockel in die Höhe. Im Sockel siehst du eine Vertiefung, in der sich ein moosgrüner Schlüssel befindet.");
		if(janeinfrage("Willst du den Schlüssel an dich nehmen (j/n)?")) {
			schluessel125 = true;
			textausgabe("Du nimmst den moosgrünen Schlüssel an dich, der offensichtlich aus Jade geschnitzt wurde. Sein kurzes Griffstück erscheint, als wäre es eine kunstvolle Darstellung der Ziffern 1, 2 & 5. Kaum daß du den Schlüssel an dich genommen hast, schraubt sich der Sockel ebenso leise wieder in den Boden, wie er sich zuvor herausgeschraubt hatte.");
		}
	}
	switch(rotation % 8) {
		case 1: auswahl(text, 6, ort68, ort81, ort83, ort84, ort85, ort86);
				break;
		case 2: auswahl(text, 6, ort67, ort81, ort83, ort84, ort85, ort86);
				break;
		case 3: auswahl(text, 6, ort74, ort81, ort83, ort84, ort85, ort86);
				break;
		case 4: auswahl(text, 6, ort73, ort81, ort83, ort84, ort85, ort86);
				break;
		case 5: auswahl(text, 6, ort72, ort81, ort83, ort84, ort85, ort86);
				break;
		case 6: auswahl(text, 6, ort71, ort81, ort83, ort84, ort85, ort86);
				break;
		case 7: auswahl(text, 6, ort70, ort81, ort83, ort84, ort85, ort86);
				break;
		default: auswahl(text, 6, ort69, ort81, ort83, ort84, ort85, ort86);
				break;
	}
}

void ort83(void) {
	rotation++;
	raum = 83;
	char *raetseltext = "Rat, wenn du kannst!\nEs nennen einen Wanst\nfünf Zeichen dir,\nund auch die letzten vier.";
	char *antworttext = "Bauch";
	textausgabe("Der Raum, in dem du dich jetzt befindest, ist aus einem einzigen großen Rubin heraus geschnitten. An der Wand befindet sich ein Symbol.");
	weiter();
	vordergrundfarbe(blau);
	textausgabe("*####_");
	textausgabe(" #");
	textausgabe("#");
	textausgabe("#");
	textausgabe(" #");
	textausgabe(" #");
	textausgabe("  V");
    vordergrundfarbe(weiss);
	if(!raetsel3) {
		textausgabe("Eine Stimme ertönt plötzlich und stellt dir eine Frage:");
		if(janeinfrage("Glaubst du, du kannst mein Rätsel lösen (j/n)?"))
			if(raetsel(raetseltext, antworttext)) {
				raetsel3 = true;
				textausgabe("Deine Antwort war ... weise!");
			} else
				textausgabe("Deine Antwort war ... unwissend!");
	}
	auswahl("Du kannst den Weg zurück nach Südwesten gehen (1) oder die Wände nach Geheimgängen absuchen (2)", 2, ort82, ort212);
}

void ort84(void) {
	rotation++;
	raum = 84;
	char *raetseltext = "Etwas, das alles und jeden verschlingt:\nBaum, der rauscht, Vogel, der singt,\nfrisst Eisen, zermalmt den härtesten Stein,\nzerbeißt jedes Schwert, zerbricht jeden Schrein,\nSchlägt Könige nieder, schleift ihren Palast,\nträgt mächtigen Fels fort als leicht Last.";
	char *antworttext = "Zeit";
	textausgabe("Der Raum, in dem du dich jetzt befindest, ist aus einem einzigen großen Lapislazuli heraus geschnitten. An der Wand siehst du ein Symbol.");
	weiter();
	vordergrundfarbe(gruen);
	textausgabe("*###*");
	textausgabe(" # #");
	textausgabe("#   #");
	textausgabe("#   #");
	textausgabe("#   #");
	textausgabe(" # #");
	textausgabe(" \\ /");
    vordergrundfarbe(weiss);
	if(!raetsel4) {
		textausgabe("Eine Stimme ertönt plötzlich und stellt dir eine Frage:");
		if(janeinfrage("Glaubst du, du kannst mein Rätsel lösen (j/n)?"))
			if(raetsel(raetseltext, antworttext)) {
				raetsel4 = true;
				textausgabe("Deine Antwort war ... weise!");
			} else
				textausgabe("Deine Antwort war ... unwissend!");
	}
	auswahl("Du kannst den Weg zurück nach Westen gehen (1) oder die Wände nach Geheimgängen absuchen (2)", 2, ort82, ort212);
}

void ort85(void) {
	rotation++;
	raum = 85;
	char *raetseltext = "Der arme Tropf\nhat keinen Kopf;\ndas arme Weib\nhat keinen Leib,\ndie arme Kleine\nhat keine Beine.\nSie ist ein langer Darm,\ndoch schlingt sie einen Arm\nbedächtig in den anderen ein.\nWas mag das für\nein Weiblein sein?";
	char *antworttext = "Brezel";
	textausgabe("Du stehst in einem Raum, der aus eine einzigen, riesigen Tigerauge geschnitzt zu sein scheint. Du siehst ein Symbol an der Wand.");
	weiter();
	vordergrundfarbe(magenta);
	textausgabe("*##*");
	textausgabe(" ##");
	textausgabe(" ##");
	textausgabe("  #");
	textausgabe("  ## /");
	textausgabe("  # V");
	textausgabe("  /");
	textausgabe(" /");
	vordergrundfarbe(weiss);
    if(!raetsel5) {
		textausgabe("Eine Stimme ertönt plötzlich und stellt dir eine Frage:");
		if(janeinfrage("Glaubst du, du kannst mein Rätsel lösen (j/n)?"))
			if(raetsel(raetseltext, antworttext)) {
				raetsel5 = true;
				textausgabe("Deine Antwort war ... weise!");
			} else
				textausgabe("Deine Antwort war ... unwissend!");
	}
	auswahl("Du kannst den Weg zurück nach Nordwesten gehen (1) oder die Wände nach Geheimgängen absuchen (2)", 2, ort82, ort212);
}

void ort86(void) {
	rotation++;
	char *raetseltext = "Ich hab' ein Ding im Sinn,\nwohl lieben es die Mädchen traut,\nes liegt um ihre zarte Haut,\ndoch stecken Nägel drin.";
	char *antworttext = "Handschuh";
	raum = 86;
	textausgabe("Der Raum, in dem du dich jetzt befindest, ist aus einem einzigen großen Türkis heraus geschnitten.");
	weiter();
	vordergrundfarbe(zyan);
	textausgabe("  #");
	textausgabe("   #");
	textausgabe("##  #");
	textausgabe(" ###");
	textausgabe(" ##");
	textausgabe("  #");
	textausgabe("  #");
	textausgabe(" \\/");
	vordergrundfarbe(weiss);
    if(!raetsel2) {
		textausgabe("Eine Stimme ertönt plötzlich und stellt dir eine Frage:");
		if(janeinfrage("Glaubst du, du kannst mein Rätsel lösen (j/n)?"))
			if(raetsel(raetseltext, antworttext)) {
				raetsel2 = true;
				textausgabe("Deine Antwort war ... weise!");
			} else
				textausgabe("Deine Antwort war ... unwissend!");
	}
	auswahl("Du kannst den Weg zurück nach Nordosten gehen (1) oder die Wände nach Geheimgängen absuchen (2)", 2, ort82, ort212);
}

void ort87(void) {
	rotation++;
	textausgabe("Du betrittst eine Naturhöhle. Das Licht hier ist intensiver, als in den meisten Gängen, die du bisher betreten hast. Büsche wachsen hier, voller Beeren, die du nicht kennst, deren Farbe aber ein deutliches Signalrot aufweisen, was kein gutes Zeichen in deiner Erinnerung ist. In deinen Ohren klingt es, als würde ein Bach oder ein Fluß in der Nähe rauschen, doch sehen kannst du von hier aus keinen. Die Höhle hat Ausgänge in drei Richtungen: einen behauenen Tunnel im Norden, eine Loch in der Ostwand und einen natürlichen Tunnel im Süden.");
	raum = 87;
	char *text = "Du kannst dem behauenen Tunnel nach Norden folgen (1), ebenso kannst du den Südtunnel nehmen (2), oder dich dem Weg hinein in das große Loch in der Ostwand anvertrauen (3)";
	switch(rotation % 8) {
		case 1: auswahl(text, 3, ort70, ort151, ort146);
				break;
		case 2: auswahl(text, 3, ort69, ort151, ort146);
				break;
		case 3: auswahl(text, 3, ort68, ort151, ort146);
				break;
		case 4: auswahl(text, 3, ort67, ort151, ort146);
				break;
		case 5: auswahl(text, 3, ort74, ort151, ort146);
				break;
		case 6: auswahl(text, 3, ort73, ort151, ort146);
				break;
		case 7: auswahl(text, 3, ort72, ort151, ort146);
				break;
		default: auswahl(text, 3, ort71, ort151, ort146);
				break;
	}
}

void ort88(void) {
	rotation++;
	if(!(raum == 88) && !(raum = 145) && !(raum == 89))
		textausgabe("Nach einiger Zeit verändert sich der Tunnel und wird zu einem Stollen. Der Ort wo du jetzt erinnert mit seinen Balkenkonstruktionen sehr an ein Bergwerk. Die Beleuchtung ist jetzt viel spärlicher.");
	raum = 88;
	char *text = "Du kannst dem Tunnel nach Norden (1) oder nach Süden folgen (2), odern den Weg nach Westen nehmen (3)";
	textausgabe("Du befindest dich an einer Kreuzung. Ein Tunnel führt von Norden nach Westen, während ein weiterer Stollen in Richtung Westen führt. Das Wurzelwerk in den Wänden schimmert weniger intensiv, als du es bisher gewohnt bist. Ein Schienenstrang führt an der Wand des nach Westen laufenden Gangs entlang.");
	switch(rotation % 8) {
		case 1: auswahl(text, 3, ort71, ort89, ort145);
				break;
		case 2: auswahl(text, 3, ort70, ort89, ort145);
				break;
		case 3: auswahl(text, 3, ort69, ort89, ort145);
				break;
		case 4: auswahl(text, 3, ort68, ort89, ort145);
				break;
		case 5: auswahl(text, 3, ort67, ort89, ort145);
				break;
		case 6: auswahl(text, 3, ort74, ort89, ort145);
				break;
		case 7: auswahl(text, 3, ort73, ort89, ort145);
				break;
		default: auswahl(text, 3, ort72, ort89, ort45);
				break;

	}
}

void ort89(void) {
	rotation++;
	raum = 89;
	textausgabe("Du betrittst einen Raum, dessen Wände abgeschliffen und vollkommen Glatt sind. Nichts, nicht einmal Staub oder Schmutz, sind in diesem Raum sichtbar.");
	if(wuerfel(6) > 3)
		textausgabe("Dich beschleicht ein mulmiges Gefühl.");
	auswahl("Du kannst den Raum durch den Ausgang im Norden verlassen (1) oder durch jenen im Süden (2). Wenn du tatsächlich glaubst, du könntest vielleicht einen Geheimgang oder eine Geheimtüre finden, so kannst du stattdessen auch die Wände absuchen (3)", 3, ort88, ort90, ort212);
}

void ort90(void) {
	rotation++;
	raum = 90;
	textausgabe("Der Raum den du betrittst verschlägt dir die Sprache. Er ist über und über voll mit Schnitzereien und Verzierungen im Fels und wie es aussieht, wurde einem riesigen Wandbild mit Hilfe von Edelsteinen Farbei eingehaucht. Es läßt Meisterwerke der Malerei neben sich zu einem Nichts verblassen. Wenn kein Erdbeben dieses Kunstwerk vernichtet, wird man es vermutlich noch in Jahrmillionen betrachten können. In der Ostwand ist ein beeindruckender, gigantischer Torrahmen.");
	if(gitteroffen) {
		textausgabe("Das Tor steht offen - doch kann man ganz obem am Türrahmen die feinen Verfärbungen des Gitters im Gestein sehen, das nur darauf zu lauern scheint, auf dich herabzustürzen, wenn du versuchst hindurchzugehen.");
		auswahl("Du kannst dem Weg nach Norden einschlagen (1) oder das gigantische Tor nach Osten durchschreiten (2). Wenn du es für nötig hältst, kannst du jedoch auch die Wände nach Geheimgängen absuchen (3)", 3, ort89, ort91, ort212);
	}
	else {
		textausgabe("Zwar steht das gigantische Tor offen, seine Türen sind weit aufgeschwungen, auch wenn du dir nicht vorstellen kannst, das irgendwer diese Flügel auch nur einen einzigen Millimeter weit bewegen könnte, dennoch kannst du nicht hindurch, den ein riesiges Gitter ist herabgelassen worden - und erlaubt nur den kleinsten Lebewesen ein Durchkommen. Klein - in der Größenordnungen von Katzen, Hunden oder Nagern. Du würdest es hingegen nicht einmal schaffen, deinen Kopf hindurchzuzwängen.");
		auswahl("Du kannst den Weg zurück nach Norden gehen (1) oder die Wände nach Geheimgängen absuchen (2)", 3, ort89, ort212);
	}
}

void ort91(void) {
	rotation++;
	raum = 91;
	textausgabe("Der Raum den du betrittst verschlägt dir die Sprache. Seine Wände scheinen von innen heraus zu leuchten, obwohl sie aus Metall zu scheinen sein. In die Wände hinein wurden Bilder geschnitzt, die durch das Leuchten der Wände irgendwie den Eindruck erwecken, als würden sie leben. An der Decke ist ein riesiges Bild dargestellt, dessen Sinn du nicht genau erkennen kannst, denn es ist aus Edelsteinen erstellt und sein Anblickt schmerzt geradezu in deinen Augen, so daß dir die Tränen kommen. In der Westwand ist ein beeindruckender, gigantischer Torrahmen.");
	if(gitteroffen) {
		textausgabe("Das Tor steht offen - doch kann man ganz obem am Türrahmen die feinen Verfärbungen des Gitters im Gestein sehen, das nur darauf zu lauern scheint, auf dich herabzustürzen, wenn du versuchst hindurchzugehen.");
		auswahl("Du kannst den Weg durch das Tor nach Westen nehmen (1) oder den Ausgang nach Osten nehmen (2). Wenn du es für nötig hältst, kannst du jedoch auch die Wände nach Geheimgängen absuchen (3)", 3, ort90, ort92, ort212);
	}
	else {
		textausgabe("Zwar steht das gigantische Tor offen, seine Türen sind weit aufgeschwungen, auch wenn du dir nicht vorstellen kannst, das irgendwer diese Flügel auch nur einen einzigen Millimeter weit bewegen könnte, dennoch kannst du nicht hindurch, den ein riesiges Gitter ist herabgelassen worden - und erlaubt nur den kleinsten Lebewesen ein Durchkommen. Klein - in der Größenordnungen von Katzen, Hunden oder Nagern. Du würdest es hingegen nicht einmal schaffen, deinen Kopf hindurchzuzwängen.");
		auswahl("Du kannst den Weg zurück nach Osten gehen (1) oder die Wände nach Geheimgängen absuchen (2)", 3, ort92, ort212);
	}
}

void ort92(void) {
	rotation++;
	textausgabe("Du stehst in einem perfekt aus dem Felsen geschnitzten Tunnelstück, das von Westen nach Osten führt.");
	raum = 92;
	auswahl("Du kannst dem Tunnel nach Westen folgen (1) oder nach Osten (2) oder die Wände nach Geheimgängen absuchen (3)", 3, ort91, ort93, ort212);
}

void ort93(void) {
	rotation++;
	textausgabe("Du stehst an einer Abbiegugn des Tunnels, die von Norden nach Westen verläuft.");
	raum = 93;
	auswahl("Du kannst dem Weg nach Norden folgen (1) oder nach Westen (2) oder die Wände nach Geheimgängen absuchen (3)", 3, ort94, ort92, ort212);
}

void ort94(void) {
	rotation++;
	textausgabe("Du hast das Ende des aus dem Felsen geschnitzten Tunnels erreicht. Die Wände hier sind mit Motiven eines Drachens dargestellt. Er ist von roter Farbe - und wie es aussieht, hat er einen Hort, den er bewacht. Viele Schriftzeichen bedecken die Wände in langen Reihen. Du weißt nicht warum, aber du kannst dich des unbestimmten Gefühls nicht erwehren, gerade in einer Art Grabkammer zu stehen, auch wenn du kein einziges der Schriftzeichen an der Wand auch nur im entferntesten deuten kannst.");
	raum = 94;
	// Die Chance den richtigen Ausgang zu finden ist 
	auswahl("Dir steht nur der Weg zurück durch den Felstunnel offen (1). Du kannst aber selbstverständlich versuchen, die Wände nach Geheimgängen abzutasten (2).", 2, ort93, (wuerfel(6) > 4) ? ort142 : ort95);
}

void ort95(void) {
	rotation++;
	if((raum == 74) || (raum == 142) || (raum == 99) || (raum == 97) || (raum == 98) ) {
		textausgabe("Du entdeckst einen Geheimgang und kriechst in ihn hinein. Nur mühsam, auf Knien und Handflächen, robbst du durch den engen Tunnel. Mermalst drohst du, die Orientierung zu verlieren, doch schließlich hat die Dunkelheit und mit ihr die Orientierungslosikeit ein Ende. Du gelangst in einen hell erleuchteten Raum. Eigentlich, wenn man den geheimen Tunnel nicht bemerkt, ist der Raum eine Art Sackgasse. Aus ihm führt nur ein Gang hinaus.");
		raum = 95;
		auswahl("Du kannst dem Gang hinaus folgen (1) oder dich umdrehen und versuchen, den Weg durch den Geheimgang zurückzufinden (2)", 2, ort134, ort74);
	} else { // raum == 134
		raum = 95;
		textausgabe("Du gelangst an das Ende des Ganges. Enttäuscht mußt du feststellen, das hier nur unbehauene Wände und sonst rein gar nichts ist.");
		auswahl("Willst du unverrichteter Dinge umkehren (1) oder die Wände nach Geheimgängen absuchen (2)?", 2, ort134, ort74);
	}
}

void ort96(void) {
	rotation++;
	raum = 96;
	if(arianna == 99) {
		zweisamkeit(1);
		textausgabe("Als du das erste Mal diesen Ort betreten hast, da war er für dich nur eine Höhle, in der ein Lavabecken stand - und ein seltsamer Metalklumpen, der die eigenartig glatt erschien. Aber jetzt weißt du, was er in Wahrheit darstellt. Das hier ist eine echte Zwergenschmiede, eine Schmiede, in der im heißesten aller Feuer, der Bund für die Ewigkeit besiegelt wird. Alle Zwerge aus Dwellmerdan tummeln sich hier in der Schmiede. Du bahnst dir unter freundschaftlichen Schulterklapsen deinen Weg durch die Meute, nach vorne, wo der Dorfälteste und der Schmied dich bereits erwarten. Und dann wird es leise, etwas, was nur selten bei Zwergen vorkommt - und Arianna wird in Begleitung ihrer Mutter und ihres Vaters zum Amboß geführt. Lansam, ruhig und gemessenen Schrittes nähern sich die drei, während ein Chor aus Sackpfeifen und Blechbläsern die zwergische Hochtzeitsweise anstimmt.\nUnd plötzlich stehen Arianna und ihre Eltern nur noch einen Schritt entfernt vor dir. Das Gesicht ihres Vaters ist Rot vor Zorn. Er holt aus - und schlägt dir mit aller Wut ins Gesicht. Du knickst ein, doch als du aufstehst, tust du es im gleich. Mit all der Kraft, die die Liebe zu Arianna dir verleiht, schlägst du zu - und du hast Glück. Du triffst ihren Vater nicht nur, du fällst ihn wie einen Baum. Er knickt nicht einmal in den Beinen ein, sondern kippt unter dem Gröhlen der Zwerge hinten über. Du Mutter und ein paar herbeieilende Dwellmer packen ihn am Kragen und ziehen ihn in die Reihe der Feiernden, die bereits damit begonnen haben, Met aus ihren Gürtelschläuchen zu saufen. Als nächstes legen Arianna und du eure Oberbekleidung ab. Mit nackten Oberkörpern steht ihr voreinander. Zwei Zwerge legen euch mit Zangen die eisernen Bande um die Handgelenke. Der Schmerz ist so überwältigend, daß dir Tränen in die Augen schießen. Und schon hämmert Arianna auf dein Armband ein, verschließt es mit kräftigen Hammerschlägen, während die Zwerge dazu ihr Lied des ewigen Bundes anstimmen. Diesmal klingt es viel volltönender, da noch keiner der Zwerge zu besoffen zum singen ist.\nNachdem du den Schock des Schmerzes überwunden hast - und Arianna dich noch einmal flüsternd daran erinnert hat, beginnst auch du damit, mit deiner freien Hand und einem Schmiedehammer auf ihr Band einzuhämmer und verschließt es. Hinterher kannst du gar nicht mehr sagen, wie lange es gedauert hat, doch als ihr endlich fertig wart mit dem Hämmern, da leuchteten Runen in den Armbändern für Bruchteile einer Sekunde auf - und besiegelten so für immer euer Bündnis. Jetzt stürmen auch die anderen Zwerge auf euch ein, reichen euch die Schläuche voller Met und saufen mit euch um die Wette.");
	}
	auswahl("Du kannst den Weg entweder zurück nach Norden gehen (1) oder die Wände nach Geheimgängen absuchen (2)", 2, ort142, ort97);
}

void ort97(void) {
	rotation++;
	if((raum == 96) || (raum == 98))
		textausgabe("Du bewegst dich für eine lange Zeit durch einen dunklen Gang. Er ist eng und an einigen Stellen mußt du sogar auf allen Vieren weiterkrabbeln. Es ist stockfinster und du verlierst vollkommen die Orientierung. Mehr als nur einmal hast du das Gefühl, in mehr als eine Richtung weitergehen zu können. Schließlich gelingt es dir, einen Ausgang zu finden. Du wirst geblendet von dem Licht und brauchst einige Momente um dich zurechtzufinden, erst dann kannst du erkennen, wo du bist.");
	raum = 97;
	textausgabe("Du befindest dich in einem Wald aus Pilzen der verschiedensten Größe. Weit entfernt in südlicher Richtung kannst du einen Pilz gigantischer Größe ausmachen, der alle anderen überragt. Helles Licht entströmt den Lamellen unter seinem gigantischen Pilzhut, er scheint die Lichtquelle dieser Höhle zu sein.");
	auswahl("Du kannst dich entweder in westliche Richtung fortbewegen (1) oder nach Süden (2). Wenn du etwas Zeit investierst, findest du vielleicht auch noch einen Weg in eine weitere Richtung (3)", 3, ort148, ort154, (wuerfel(6) > 3) ? ort96 : ort95);
}

void ort98(void) {
	static charakter_t drache = { "Drache", 20, 20, 20, 20 };
	rotation++;
	if((raum == 99) || (raum == 74))
		textausgabe("Du findest eine Spalte im Schatten der Wand. Sie ist äußerst schmal, aber es gelingt dir, dich hineinzuquetschen. So kriechst du auf Händen und Füßen weiter - und plötzlich wird dir klar, das nach all den kleinen Windungen, du dich ja gar nicht umdrehen kannst. Zeitweise mußt du sogar flach auf den Boden gepreßt, die Arme vorausgestreckt, weiterkriechen um nicht steckenzubleiben. Die Minuten werden für dich zu kleinen Ewigkeiten, die Sekunden dehnen sich zu Stunden aus. Und endlich - sind deine Arme frei, du kriechst schneller, kletterst aus dem beengenden Gang heraus und richtest dich auf, wobei deine Glieder heftig maulend über die vorhergehende Position reagieren. Du reckst und streckst dich, so gut es eben geht. Es ist immer noch so dunkel wie in dem endlosen Gang um dich herum, aber du spürst trotzdem eine Veränderung. Es fühlt sich an, als wäre die Luft wärmer geworden. Aber vermutlich ist daß nur Einbildung. Tatsächlich war der Kriechgang anstrengend und du bist ganz schön ins Schwitzen gekommen.");
	if(raum == 162)
		textausgabe("Während du dich weiter durch den stockdunklen Gang nach Westen tastest, spürst du eine Veränderung in der der Umgebung. Die Luft fühlt sich wärmer an. Sie riecht anders, verbrauchter. Und die Klänge der Hall deiner Schritte, alles klingt, als wäre jetzt mehr Raum da.");
	raum = 98;
	if(drachetot)
		textausgabe("Der Leichnam des Drachen glüht von innen heraus und läßt die Wände um dich herum erstrahlen.");
	else if (dracheverletzt)
		textausgabe("Eine Atemflamme des Drachens erfaßt die Decke. Phosphoriszierende Steine in den Wänden reagieren auf den blendenden Feuerstrahl, während deine Augen wieder tränen. Der Drache steht da. Sein Gesichtsausdruck hat sich nicht verändert, aber dort, wo dein Pickel sein Auge trag, ist jetzt nur eine blutige Höhle. Du stehst so günstig, das er dich noch nicht gesehen hat. Allerdings bläht er seine Nüstern auf. Er scheint dich riechen zu können.");
	else
		textausgabe("In der Dunkelheit erscheint plötzlich ein Auge, dessen pupille geschlitzt ist. Die Iris erscheint in rötlichem Feuer zu strahlen. Das Auge ist groß, sehr groß - größe als deine Handfläche. Und als es sich zur Seite wendet, erscheint ein zweites. Und das ungute Gefühl überkommt dich, das es mit den beiden Augen in der Dunkelheit nichts Gutes hat. Dann erheben sich die Augen, scheinen immer weiter nach oben zu schweben. Du hörst ein Geräusch, als würde ein Elefant sich auf einen heftigen Niesser vorbereiten - und plötzlich schießt eine Lichtsäule gegen die Decke. Es riecht verbrannt. Du mußt deine Augen schützen, fühlst dich geblendet. Tränen laufen aus ihnen heraus. Du glaubst ein Schnauben wahrzunehmen.");
	textausgabe("Die Wände leuchten in einem fahlen rot, das von den phosphoriszierenden Adern darin hervorgeht.");
	// Ich will ja nicht den Spieler absichtlich töten
	if(!dracheverletzt && (objekt[adamantpickel] < 1)) {
		if(objekt[patrone] > 0)
			textausgabe("Du schießt einmal beherzt auf den Drachen, bist dir allerdings nicht sicher, das er es wahrgenommen hat. Vermutlich hatte die Patrone noch etwas weniger Wirkung, als ein Watteball, der gegen einen Baum stößt.");
		textausgabe("Du nutzt die Gelegenheit, als der Drache erneut Luft holt, um die Beine in die Hand zu nehmen und wie von der Tarantel gestochen in den dunklen Gang hineinzulaufen. Mit ein wenig Glück, überlebst du so vielleicht.");
		ort162();
	}
	if(objekt[adamantpickel] > 0) {
		if(tdg(&spieler))
			textausgabe("Als der Kopf des Drachens nah genug an dich herangekommen ist, holst du mit deiner Spitzhacke aus und rammst sie ihm mit voller Wucht ins Auge. Es spritzt - und sein Kopf ruckt sofort weg von dir. Die scheußlichen Töne, die er jetzt von sich gibt, sind vermutlich sein Wehklagen.");
		else
			textausgabe("Als der Koopf des Drachens nah genug an dich herangekommen ist, holst du mit deiner Spitzhacke aus und haust ins Leere. Der Drache hat blitzschnell reagiert und den Kopf zurückgezogen.");
	}
	if(!drachetot && janeinfrage("Willst du den günstigen Moment nutzen, die Beine in die Hand nehmen - und so schnell du nur kannst von hier fliehen (j/n)?"))
		ort162();
	if(!drachetot && !dracheverletzt) {
		textausgabe("Der Drache dreht sich blitzschnell um die eigene Achse. Du siehst noch, wie sein Schwanz heranrast, dann wirst du auch schon von ihm getroffen. Der Schmerz ist fürchterlich. Im weiten Bogen fliegst du rücklinks in die Dunkelheit hinein.");
		// schwerer Treffer, eventuell tödlich
		staerkesteigerung(-5,0);
		if(spieler.staerke <= 0)
            beenden(rot, EXIT_SUCCESS, "Du hörst es laut krachen, als du gegen die Felswand schmetterst. Alle Knochen tun dir weh. Am liebsten würdest du jetzt stöhen, aber du fühlst dich viel zu kraftlos. Dann siehst du eine Wand aus Feuer, wie sie durch den Tunnel auf dich zuströmt. Deine Augen tränen und alles wird Schwarz, bis auf den beißenden, brennenden Schmerz, der erst nachläßt, als du dein Leben losläßt. Die Begegnung mit dem Drachen hat dein Leben beENDEt.");
	}
	if(dracheverletzt && !drachetot) {
		drache.gewandheit = 16;
		drache.staerke = 15;
	}
	if(!drachetot) {
		if(!kampf(&spieler, &drache, 1, false, ort162))
            beenden(rot, EXIT_SUCCESS, "Du hörst es laut krachen, als du gegen die Felswand schmetterst. Alle Knochen tun dir weh. Am liebsten würdest du jetzt stöhen, aber du fühlst dich viel zu kraftlos. Dann siehst du eine Wand aus Feuer, wie sie durch den Tunnel auf dich zuströmt. Deine Augen tränen und alles wird Schwarz, bis auf den beißenden, brennenden Schmerz, der erst nachläßt, als du dein Leben losläßt. Die Begegnung mit dem Drachen hat dein Leben beENDEt.");
		textausgabe("Ein Ruck geht durch den Körper des Drachen, ein letztes Aufbäumen, dann sinkt er tot zusammen. Sein Körper sieht aus, als würde er glühen. Vielleicht verbrennt er ja innerlich, du weißt es nicht. Das hier war der erste Drache, den du in deinem Leben gesehen hast. Und du dachtest immer, Drachen würden in das Reich der Märchen gehören. Die Legende von der Unverwundbarkeit durch Drachenblut jedenfalls hast du wiederlegt. Dein linker Arm ist nur so besudelt von Drachenblut, trotzdem hast du lange Schnittwunden darauf.\nPlötzlich mußt du lachen, als du bemerkst, das jemand ein Wort auf die Unterseite des Drachens gemalt hat: \"Bauch\"\nEin ganz klares Zeichen dafür, das du nicht der erste bist, der sich hier in die Höhle des Drachens gewagt hat.");
		// Ich denke, an der Legende ist doch ein Körnchen Wahrheit
		staerkesteigerung(0,1);
	}
	auswahl("Du kannst jetzt dem Tunnel weiter nach Westen folgen (1) oder nach Osten gehen (2) oder die Wände nach Geheimgängen absuchen (3)", 3, ort161, ort162, (wuerfel(6) > 3) ? ort99 : ort95);
}

void ort99(void) {
	rotation++;
	if(raum == 98)
		textausgabe("Du findest eine Spalte in der Wand und nutzt die Gunst des Augenblicks, so schnell du kannst auf Händen und Knien hineinzurobben. Von Panik und Angst getrieben kriechst du weiter durch einen endlos lang erscheinenden Stollen. Manchmal musst du dich sogar auf den Boden pressen und dich liegend voranschieben. Deine Angst ist groß, stecken zu bleiben. Dein Herz ist immer noch wie wild am Pochen, als der Kriechgang, durch den du dich seit Minuten gequetscht hast, sein Ende findet. Glücklich darüber, endlich wieder mehr Platz um dich herum zu haben, krabbelst du aus dem Loch heraus und richtest dich auf.");
	raum = 99;
	textausgabe("Der Stollen fällt hier stärker von Norden nach Süden ab. Ein paar Schienen laufen an der Ostwand entlang. Das Wurzelwerk breitet sich nur sehr spärlich an der Decke aus und entsprechend schlecht ist die natürliche Sicht hier unten. Die Struktur der Wände an der Westwand ist sehr zerklüftet, scharfkantige Strukturen stehen hervor und der Schattenwurf ist großflächig.");
	if(wuerfel(6) > 3)
		textausgabe("Dir kommt es so vor, als würdest du Klopfgeräusche aus dem Stollen kommen hören.");
	if(wuerfel(6) > 3)
		textausgabe("Es kommt dir so vor, als hätte sich das Gestein des Felsens leicht verändert.");
	if(wuerfel(6) > 4)
		textausgabe("Du bist dir nicht sicher, aber trotz des schwachen Lichts hast du das Gefühl, kleine metallische Lichtreflexe auf der Felswand erkennen zu können.");
	auswahl("Du kannst dem Tunnel nach Norden folgen (1) oder nach Süden (2) oder die Wände nach Geheimgängen absuchen (3)", 3, ort149, ort158, (wuerfel(6) > 3) ? ort98 : ort95);
}

// -----------
// --- 100 ---
// -----------

void ort100(void) {
// Du stimmst Elke zu und ihr verlaßt das Parkhaus
	raum = 100;
	textausgabe("Ihr verlaßt das Auto. Elke fährt den Wagen die Stepgesstraße hinunter und hält vor der Ampel. Sobald Grün kommt, überquert sie die Kreuzung und fährt den Berliner Platz entlang. Du schaust hinüber zum Kaiserbad, wo du als kleines Kind schwimmen gelernt hast.\n\"Schaltest du mal das Radio an?\" bittet sie dich und du wendest dich der Mittelkonsole zu. Kurz darauf ist das Digitalradio an. \"Mit Radio 90,1 wird das nichts. Der Sender hat wohl keinen Saft.\" kommentierst du den vorausgewälten Sender und probierst einen Senderplatz nach dem anderen. Schließlich empfängt das Radio ein Signal auf der Deutschen Welle.\n\"... wird im Westen der Republik von verheerenden Unwettern berichtet, die auch im Venloer Raum bereits für erhebliche Zerstörung gesorgt haben sollen. Fast die ganze Niederlande und der Niederrhein verfügen über keinerlei Strom mehr. Auch die offiziellen Behördennetzwerke in dem Gebiet können nicht durch die Bundesregierung erreicht werden. Derweil hat der Kanzler eine Mobilmachung der Bundeswehr angeordnet, die eine Versorgung des Gebietes erreichen soll. Aufnahmen von Wettersatelliten zeigen in der Region nichts weiter als eine ausnehmend großen Masse an dichten dunklen Wolken....\"\nIm nächsten Augenblick hatte das Radio auch die Deutsche Welle verloren. Der Wagen rollte derweil die Aachener Straße in Richtung Rheindahlen, Wegberg, Erkelenz entlang. Sie kamen am neuen Bundesliga-Stadion vorbei, doch die Sicht wurde jetzt weiter, offener. Überall im Westen näherte sich eine riesige, bedrohliche Wolkenwand. Selbst im Süden konnte man jetzt sehen, daß sie auch dort waren. Im Osten hingegen kam nun ein kleiner Spalt hellblauen Himmels zum Vorschein. Elke fuhr mit dem Auto auf einen Wendeplatz und hielt den Wagen an.\n\"Was denkst du, wohin sollen wir jetzt fahren?\"");
	auswahl("Wenn du weiter nach Süden willst (1), nach Westen, wo die Wolken noch viel näher und finsterer sind (2), Osten, wo ein Streifen blauer Himmel jetzt sichtbar wird (3), oder zurück nach Mönchengladbach, wo die Turmmonster sind, du dich aber auskennst (4).", 4, ort106, ort107, ort108, ort109);
}

void ort101(void) {
// Du hast ein mulmiges Gefühl und versuchtst Elke vom rausfahren abzuhalten
	raum = 101;
	if(tripodgesehen == true) {
		textausgabe("Du erzählst Elke alles, was du gesehen hast, und beschreibst ihr verbal dieses turmgroße Etwas, das sich wie auf Stelzen zu bewegen schien und von dem aus Lichtbögen die Umgebung zerstörten.\nElke sieht dich irritiert an und fragt schließlich, wieso du ihr das nicht schon früher gesagt hättest? Schließlich bricht sie in schallendes Gelächter aus. Es vergeht eine Minute, bis sie sich wieder unter Kontrolle hat.\n\"Ich denke, du bist ein echt lieber Kerl, aber auch ein ziemlicher Phantast, hm?\"\nSie wirft den Motor an. Du glaubst deiner Erinnerung auch nicht wirklich - und irgendwie scheint es sowieso egal zu sein. Im Moment ist einzig und allein am wichtigsten, einmal etwas über die aktuelle Lage in Erfahrung zu bringen. Langsam rollt der Wagen durch das Parkhaus.");
	}
	else {
		textausgabe("Du erzählst ihr, das du ein ungutes Gefühl hast, ob es nun an dem schlechten Wetter, den Sirenen oder was auch immer liegt, kannst du nicht sagen, aber du bringst klar zum Ausdruck, daß du kein gutes Gefühl dabei hast, in einem Auto auf die Straßen zu fahren, nicht jetzt, nicht bei diesem Wetter, der Situation - und erst recht nicht, wenn es nach deinem Bauchgefühl geht. Aber Elkes Einwände sind besser, immerhind könntet ihr ein wenig erfahren, wieso die ganze Stadt einen Stromausfall erfahren hat. Du zuckst noch einmal mit den Schultern und willigst ein, schnallst dich an und schon geht es aus der Parklücke heraus und ihr rollt durch das dunkle Parkhaus.");
	}
	textausgabe("Plötzlich gibt es so etwas wie einen kurzen, heftigen Erdstoß. Von einigen wenigen Autos schaltet sich die Diebstahlwarnanlage and und eröffnet eine neue Kakophonie innerhalb des Parkhauses. Jetz wäre dir wirklich nichts lieber, als endlich hier heraus zu sein. Ihr nähert euch der Ausfahrt. Elke schiebt ihre Karte in den alten Scheidt&Bachmann, dann öffnet sich die Schranke. Ihr fahrt langsam an - und verlaßt das Parkhaus. Elke will gerade abfahren, als zum zweiten Mal ein Erdstoß erfolgt.\n\"Gib bitte Gas!\" sagst du zu ihr, mit einem mummeligen Gefühl im Magen. Elke nickt und fährt los.");
	versuchedeinglueck(ort103, ort102);
}

void ort102(void) {
// Kein Glück - es hat Elke erwischt
	raum = 102;
	textausgabe("Mit einem fürchterlichem Geräusch von schneidendem Metall, einer unglaublichen Erschütterung und einem unbeschreiblichem, donnernden Geräusch, wird ein Teil eures Autos zerteten, durchgeteilt, vernichtet. Ein riesiger Metallfuß, wie du ihn von AT-AT aus dem Kino kennst, hat einen Teil des Motorblocks und die Fahrerseite zerquetscht. Du selber bist auch etwas eingequetscht. Dein Herz rast vor Angst und entsetzen. Neben dir, daß weißt du instinktiv, hat gerade Elke ihren Tod gefunden. Obwohl du sie kaum kanntest, tut dir das in der Seele weh, du weinst - versuchst aber gleichzeitig keinen Mucks zu machen - um nicht das gleiche Schicksal zu erleiden. In deiner verzweifelten Lage klappst du den Sitz langsam nach hinten. Du schaffst es, die Befestigung des Rücksitzes zu lösen - und kannst so deinen Rucksack aus dem Kofferraum holen. Du holst zusätzlich noch die Beutel die du erreichen kannst und stopfst dir deinen Rucksack damit voll. Du versuchst nicht zur Seite zu schauen, dir ein Bild von Elke zu machen.");
	auswahl("Willst du dich weiter, so leise wie möglich verhalten (1) oder willst du versuchen, aus dem Auto herauszukommen - und zu fliehen. Immerhin könnte der riesige Fuß ja noch ein zweites Mal auf das Auto herabsausen (2)?", 2, ort104, ort105);
}

void ort103(void) {
// Glück gehabt
	raum = 103;
	textausgabe("Keine zwei Meter von eurem Auto entfernt, knallt ein riesiger Metallfuß mit ohrenbetäubendem Lärm und unter einer kurzen, heftigen Erschütterung, auf den Boden. Ihr werdet beide blass - doch sofort brüllst du Elke an: \"Fahr! Fahr! Fahr!\"\nSie löst sich aus ihrer Schockstarre und tritt das Gaspedal bis zum Anschlag durch. Mit einem Satz rollt ihr die Stepgesstraße hinab. Ohne auf die Ampelanlage zu achten, schießt ihr über die Kreuzung, am Kaiserbad vorbei in Richtung Korschenbroicher Straße, mit quietschenden Reifen biegt ihr in die Fliethstraße ein, während Elke immer weiter beschleunigt. Und du hast nichts dagegen einzuwenden, denn auch du willst nichts weiter, als weg. Ihr rast am Geroweiher vorbei, als du brüllst, sie soll nach Süden abbiegen. So rast ihr nahezu unaufhalsam über die Kreuzung und dann die Aachener Straße in entlang in Richtung Rheindalen. Für den Moment überlegst du ob ihr auf die Autobahn sollt, aber von dort aus kann man gar nicht mehr abbiegen.\n\"Nicht auf die Autobahn\", sagst du zu ihr, \"da säßen wir in der Falle - und unser Weg wäre vorhersehbar. Als ihr über die Brücke fahrt, die über die Autobahn führt, siehst du, wie klug eure Entscheidung war. Ein weiteres dieser Turmmonster steht mitten auf der Autobahn, einen Berg aus Blechkadavern zu seinen Füßen.\n\"Wohin fahren wir jetzt?\" will sie wissen, während ihr jetzt der Straße weiter in Richtung Wegberg und Erkelenz folgt.\n\"Erst mal weiter in Richtung Süden, an dieser Unwetterwolke und den Metalltürmen vorbei. Da entdeckt ihr weit im Süden, das die Wolke sich auch dort bereits findet. Elke fährt auf den nächsten Wendeplatz und hält den Wagen an. \"Bist du dir sicher, das wir weiter nach Süden fahren wollen?\"");
	auswahl("Wenn du weiter nach Süden willst (1), nach Westen, wo die Wolken noch viel näher und finsterer sind (2), Osten, wo ein Streifen blauer Himmel jetzt sichtbar wird (3), oder zurück nach Mönchengladbach, wo die Turmmonster sind, du dich aber auskennst (4).", 4, ort106, ort107, ort108, ort109);
}

void ort104(void) {
// Im Auto bleiben
    beenden(rot, EXIT_SUCCESS, "Du verhältst dich muskmäuschen still. Den Rucksack hast du auf dem Schoß. Du atmest nur gan flach, schluchzt aber auch immer wieder. Du bedauerst deine eigene verzweifelte Lage noch viel mehr, als den Tod der freundlichen Frau, die hier, nur wenige Zentimeter entfernt von dir, unter einem tonnenschweren Stahlfuß zerquetscht wurde. Plötzlich hörst rechts neben dir ein Geräusch. Erschreckt siehst du zur Seite - und starrst in die Mündung eines Automatikgewehres.\nBefriedigt grunzt das Wesen, das gerade seine Waffe in deinem Kopf entladen hat, dann wendet es sich vom Anblick deines Kadavers ab und sucht nach weiteren \"Ausreißern\".");
}

void ort105(void) {
// Flucht
	raum = 105;
}

void ort106(void) {
// Süden
	raum = 106;
}

void ort107(void) {
// Westen
	raum = 107;
}

void ort108(void) {
// Osten
	raum = 108;
}

void ort109(void) {
// Norden
	raum = 109;
}

void ort110(void) {
}

void ort111(void) {
}

void ort112(void) {
}

void ort113(void) {
	// Begegnung mit einem Zufallsgegner der "Militärseite"
	int zufallsgegner = wuerfel(6);
	bool kampfausgang;
	charakter_t gegner1 = { "Uniformierter", 2, 2, 3, 3 };
	charakter_t gegner2 = { "Besatzer", 6, 6, 3, 3 };
	charakter_t gegner3 = { "aggressiver Uniformträger", 6, 6, 4, 4 };
	charakter_t gegner4 = { "Soldat einer fremden Armee", 5, 5, 4, 4 };
	charakter_t gegner5 = { "gutgläubiger Mitläufer", 6, 6, 5, 5 };
	charakter_t gegner6 = { "Heckenschütze", 8, 8, 4, 4 };
	charakter_t gegner7 = { "gepanzerter Beserker", 6, 6, 8, 8 };
	switch(zufallsgegner) {
		case 1: kampfausgang = kampf(&spieler, &gegner1, 1, false, NULL);
				break;
		case 2: kampfausgang = kampf(&spieler, &gegner2, 1, false, NULL);
				break;
		case 3: kampfausgang = kampf(&spieler, &gegner3, 1, false, NULL);
				break;
		case 4: kampfausgang = kampf(&spieler, &gegner4, 1, false, NULL);
				break;
		case 5: kampfausgang = kampf(&spieler, &gegner5, 1, false, NULL);
				break;
		case 6: kampfausgang = kampf(&spieler, &gegner6, 1, false, NULL);
				break;
		default: kampfausgang = kampf(&spieler, &gegner7, 1, false, NULL);
				break;
	}
	if(!kampfausgang)
        beenden(rot, EXIT_SUCCESS, "Das war nicht dein bester Kampf. Um ehrlich zu sein, das war dein schlechtester Kampf - und auch dein letzter Kampf. Dein allerletzter Kampf, den du nicht überlebt hast. Mit dir ist es zu ENDE gegangen.");
	getoetetegegner += 1;
	if(wuerfel(6) >= 4) {
		if(objekt[pistole] <= 1)
			if(janeinfrage("Willst du die Pistole deines Gegners an dich nehmen (j/n)?"))
				objekt[pistole] += 1;
	}
	else {
		if(objekt[gewehr] <= 0)
			if(janeinfrage("Willst du das Gewehr deines Gegners an dich nehmen (j/n)?"))
				objekt[gewehr] += 1;
	}
	objekt[patrone] += wuerfel(8);
}

void ort114(void) {
}

void ort115(void) {
}

void ort116(void) {
}

void ort117(void) {
}

void ort118(void) {
}

void ort119(void) {
}

void ort120(void) {
}

void ort121(void) {
}

void ort122(void) {
}

void ort123(void) {
}

void ort124(void) {
}

void ort125(void) {
}

void ort126(void) {
}

void ort127(void) {
}

void ort128(void) {
}

void ort129(void) {
}

void ort130(void) {
}

void ort131(void) {
	rotation++;
	if(raum == 57) {
		raum = 131;
		auswahl("Der Gang macht hier einen Knick nach links. Willst du dem Knick folgen (1) oder dich umdrehen und zurückgehen (2)? Du kannst natürlich auch die Wände nach Geheimgängen absuchen (3).", 3, ort132, ort57, ort212);
	} else { // raum == 132
		raum = 131;
		auswahl("Der Gang macht hier einen Knick nach rechts. Willst du dem Knick folgen (1) oder dich umdrehen und zurückgehen (2)? Du kannst natürlich auch die Wände nach Geheimgängen absuchen (3).", 3, ort57, ort132, ort212);
	}
}

void ort132(void) {
	rotation++;
	if(raum == 131) {
		raum = 132;
		auswahl("Der Gang macht hier einen Knick nach rechts. Willst du dem Knick folgen (1) oder dich umdrehen und zurückgehen (2)? Du kannst natürlich auch die Wände nach Geheimgängen absuchen (3).", 3, ort138, ort131, ort212);
		
	} else { // raum == 138
		raum = 132;
		auswahl("Der Gang macht hier einen Knick nach links. Willst du dem Knick folgen (1) oder dich umdrehen und zurückgehen (2)? Du kannst natürlich auch die Wände nach Geheimgängen absuchen (3).", 3, ort131, ort138, ort212);
	}
}

void ort133(void) {
	rotation++;
	textausgabe("Der Gang in dem du dich befindest, scheint unendlich lang zu sein.");
	if(raum == 65) {
		raum = 133;
		auswahl("Du kannst dem Gang weiter folgen (1) oder umdrehen und zurückgehen (2). Wenn du willst, kannst du auch die Wände nach Geheimgängen abtasten (3)", 3, ort134, ort65, ort212);
	} else { // raum == 134
		raum = 133;
		auswahl("Du kannst dem Gang weiter folgen (1) oder umdrehen und zurückgehen (2). Wenn du willst, kannst du auch die Wände nach Geheimgängen abtasten (3)", 3, ort65, ort134, ort212);
	}
}

void ort134(void) {
	rotation++;
	textausgabe("Du kommst zu einer Stelle, an der der Gang eine Biegung macht.");
	if(raum == 95) {
		raum = 134;
		auswahl("Möchtest du der Abbiegung nach links folgen (1) oder drehst du um und gehst zurück (2)? Wenn du möchtest, kannst du auch die Wände nach Geheimgängen abtasten (3)", 3, ort133, ort95, ort212);
	} else { // raum == 133
		raum = 134;
		auswahl("Möchtest du der Abbiegung nach rechts folgen (1) oder drehst du um und gehst zurück (2)? Wenn du möchtest, kannst du auch die Wände nach Geheimgängen abtasten (3)", 3, ort95, ort133, ort212);
	}
}

void ort135(void) {
	rotation++;
	if(raum == 62) {
		raum = 135;
		textausgabe("Der Gang knickt hier nach links ab.");
		auswahl("Du kannst dem Gang weiter folgen (1) oder dich umdrehen und zurückkehren (2) oder die Wände nach Geheimgängen absuchen (3)", 3, ort136, ort62, ort212);
	} else {
		raum = 135;
		textausgabe("Der Gang knickt hier nach rechts ab.");
		auswahl("Du kannst dem Gang weiter folgen (1) oder dich umdrehen und zurückkehren (2) oder die Wände nach Geheimgängen absuchen (3)", 3, ort62, ort136, ort212);
	}
}

void ort136(void) {
	rotation++;
	if(raum == 63) {
		raum = 136;
		textausgabe("Der Gang knickt hier nach rechts ab.");
		auswahl("Du kannst dem Gang weiter folgen (1) oder dich umdrehen und zurückkehren (2) oder die Wände nach Geheimgängen absuchen (3)", 3, ort135, ort63, ort212);
	} else {
		raum = 136;
		textausgabe("Der Gang knickt hier nach links ab.");
		auswahl("Du kannst dem Gang weiter folgen (1) oder dich umdrehen und zurückkehren (2) oder die Wände nach Geheimgängen absuchen (3)", 3, ort63, ort135, ort212);
	}
}

void ort137(void) {
	rotation++;
	textausgabe("Du kommst an einen Knick im Tunnel.");
	if(raum == 138) {
		raum = 137;
		auswahl("Willst du dem Gang weiter nach links folgen (1) oder kehrt machen und den Weg zurückgehen (2)? Du kannst natürlich auch die Wände nach Geheimgängen absuchen (3).", 3, ort143, ort138, ort212);
	}
	else {
		raum = 137;
		auswahl("Willst du dem Gang weiter nach rechts folgen (1) oder kehrt machen und den Weg zurückgehen (2)? Du kannst natürlich auch die Wände nach Geheimgängen absuchen (3).", 3, ort138, ort143, ort212);
	}
}

void ort138(void) {
	rotation++;
	if(raum == 132) {
		raum = 137;
		auswahl("Du stößt auf ein Ende des Ganges. Von hier aus kannst du nur noch nach rechts (1) oder nach links (2) weitergehen, sofern du dich nicht lieber schnurstracks umdrehst, um dich nicht weiter zu verirren (3). Wenn du denkst, es könnt hier ein Geheimgang sein, kannst du natürlich auch die Wände absuchen (4)!", 4, ort137, ort139, ort132, ort212);
	}
	else if(raum == 137) {
		raum = 137;
		auswahl("Du kommst an eine Abzweiung. Du kannst weiter geradeaus gehen (1) oder nach links abbiegen (2). Natürlich kannst du dich auch einfach umdrehen und zurückgehen (3) oder nach Geheimgängen suchen (4), wenn du glaubst, hier könnte einer sein.", 4, ort139, ort132, ort137, ort212);
	} else { // raum == 139
		raum = 137;
		auswahl("Du kommst an eine Abzweigung. Du kannst weiter geradeaus gehen (1) oder nach rechts abbiegen (2). Natürlich kannst du dich auch einfach umdrehen und zurückgehen (3) oder nach Geheimgängen suchen (4), wenn du glaubst, hier könnte einer sein.", 4, ort137, ort132, ort139, ort212);
	}
}

void ort139(void) {
	rotation++;
	if(raum == 64) {
		raum = 139;
		auswahl("Der Gang macht an dieser Stelle einen Knick nach rechts. Willst du dem Knick folgen (1) oder zurückgehen (2)? Du kannst natürlich auch die Wände nach Geheimgängen absuchen (3)", 3, ort138, ort64, ort212);
	} else { // raum == 138
		raum = 139;
		auswahl("Der Gang macht an dieser Stelle einen Knick nach links. Willst du dem Knick folgen (1) oder zurückgehen (2)? Du kannst natürlich auch die Wände nach Geheimgängen absuchen (3)", 3, ort64, ort138, ort212);
	}
}

void ort140(void) {
	rotation++;
	raum = 140;
	auswahl("Du kannst den Weg zurück nach Westen gehen (1) oder die Wände nach Geheimgängen absuchen (2)", 2, ort66, ort212);

}

void ort141(void) {
	rotation++;
	raum = 141;
	auswahl("Du kannst den Weg zurück nach Osten gehen (1) oder die Wände nach Geheimgängen absuchen (2)", 2, ort142, ort212);
}

void ort142(void) {
	rotation++;
	if(raum == 94)
		textausgabe("Du kriechst durch die Schwärze eines Tunnels, eine halbe Ewigkeit lang, wie es dir erscheint, bis du schließlich ein violett schimmerndes Licht am Ende deines Tunnels erblicken kannst. So schnell du kannst, krabbelst du darauf zu.");
	raum = 142;
	textausgabe("Der Raum sieht aus wie ein großes Achteck. Die Wände und der Boden, alles deutet darauf hin, daß du in einem riesigen Amethysten stehst. Das Spiel der Farben ist atemberaubend, und doch benebelt der Anblick auch die Sinne, denn die Tiefenschärfe hat leichte Probleme, die genaue Kontur der Wände wahrzunehmen. Drei Türen befinden sich in diesem Raum. Die eine ist aus weißem Marmor geformt und befindet sich im Westen, die zweite besteht aus schwarzem Onyx und befindet sich im Süden. Im Osten ist eine Türe, die aussieht, als wäre sie aus Gold, doch verrät dir der Versuch an ihr zu kratzen, daß das Material härter ist als der härteste Stahl. Die Türe verfügt über 4 Schlüssellöcher.");
	if(dwellmer) {
		auswahl("Du kannst die Höhlen der Jagd verlassen und das Dorf der Dwellmer im Osten betreten (1), den Raum durch die Marmortüre im Westen verlassen (2) oder durch die schwarze Onyxtüre im Süden (3). Wenn dir nach einem Abenteuer ist, kannst du dich auch durch die dunklen Geheimtunnel zwängen (4)", 4, ort165, ort141, ort96, (wuerfel(6) > 3) ? ort94 : ort95);

	}
	if((schluessel9 + schluessel66 + schluessel99 + schluessel111_1 + schluessel111_2 + schluessel125) > 3) {
		if(janeinfrage("Du hast genügend Schlüssel bei dir. Möchtest du versuchen die Türe zu öffnen (j/n)?"))
			if(schluessel9 && schluessel66 && schluessel99 && schluessel125) {
				textausgabe("Du probierst die Schlüssel aus. Sie passen. Ein Schlüssel nach dem anderen läßt sich in eines der Schlüssellöcher nach dem anderen einsetzen und umdrehen. Nachdem der vierte Schlüssel eingeführt worden ist, gibt es ein klickendes Geräusch. Dieses Geräusch ist das letzte, was du wahrnimmst. Du erhältst einen Schlag auf den Kopf, dann wird alles dunkel um dich.");
				// Jetzt geht es weiter -> ins Zwergendorf
				ort175();
			}
			else
				textausgabe("Leider hast du nicht die passenden Schlüssel bei dir.");
	}
	else
		textausgabe("Du probierst erst gar nicht, die Schlüssel in die Schlösser zu stecken, da du deutlich zu wenige davon hast.");
	auswahl("Du kannst den Raum durch die Marmortüre im Westen verlassen (1) oder durch die schwarze Onyxtüre im Süden (2). Ebenso kannst du die glitzernden und funkelnden Amethystwände nach Geheimtüren absuchen (3)", 3, ort141, ort96, (wuerfel(6) > 3) ? ort94 : ort95);
}

void ort143(void) {
	rotation++;
	raum = 143;
	textausgabe("Die Wände dieses Raumes scheinen die Funktionsfähigkeit deines Kompasses zu beeinflussen, die Nadel rotiert wie wild. Der Raum in dem du jetzt stehst, wird an seinem einen Ende durch eine herabstürzende Wand aus Wasser begrenzt, während auf der anderen Seite ein Tunnel in den Felsen führt.");
	auswahl("Du kannst dem Tunnel folgen (1) oder versuchen die Wasserwand zu durchqueren(2). Wenn du glaubst, du könntest einen Geheimgang finden, dürfte hier der ideale Ort zum Suchen sein (3).", 3, ort137, ort80, ort212);
}

void ort144(void) {
	rotation++;
	raum = 144;
	textausgabe("Der Stollen fällt langsam von Osten nach Westen ab, er macht an dieser Stelle einen Knick von Osten nach Süden. Ein paar Schienen laufen an der Südwand entlang und biegen dann schließlich ganz nach Süden ab, wo sie an der Ostwand weiterführen. Das Wurzelwerk breitet sich nur sehr spärlich an der Decke aus und entsprechend schlecht ist die natürliche Sicht hier unten.");
	if(wuerfel(6) > 4)
		textausgabe("Dir kommt es so vor, als würdest du Klopfgeräusche aus dem Stollen kommen hören.");
	if(wuerfel(6) > 4)
		textausgabe("Es kommt dir so vor, als hätte sich das Gestein des Felsens leicht verändert.");
	if(wuerfel(6) > 5)
		textausgabe("Du bist dir nicht sicher, aber trotz des schwachen Lichts hast du das Gefühl, kleine metallische Lichtreflexe auf der Felswand erkennen zu können.");
	auswahl("Du kannst nach Osten gehen (1) oder nach Süden (2) oder die Wände nach Geheimgängen absuchen (3)", 3, ort145, ort149, ort212);
}

void ort145(void) {
	rotation++;
	raum = 145;
	textausgabe("Der Stollen fällt langsam von Osten nach Westen ab. Ein paar Schienen laufen an der Südwand entlang. Das Wurzelwerk breitet sich nur sehr spärlich an der Decke aus und entsprechend schlecht ist die natürliche Sicht hier unten.");
	if(wuerfel(6) > 5)
		textausgabe("Dir kommt es so vor, als würdest du Klopfgeräusche aus dem Stollen kommen hören.");
	auswahl("Du kannst dem Tunnel nach Westen folgen (1) oder nach Osten (2) oder die Wände nach Geheimgängen absuchen (3)", 3, ort144, ort88, ort212);
}

void ort146(void) {
	rotation++;
	raum = 146;
	textausgabe("Du befindest dich in einer riesigen Höhle. Sie ist so riesig, das du nur im Westen eine Wand wahrnehmen kannst. Umgeben bist du von einer Art Wald, aber es ist kein natürlicher Wald, wie du ihn von der Oberfläche her kennst. Es ist ein Funghi-Wald. Pilze, kleine Pilze, wie Champions wachsen hier so zahlreich, das es unmöglich erscheint, nicht auf sie zu treten, aber auch Pilze in der Größe von Mäusen, von Katzen. Doch es sind auch größere Exemplare da, Pilze in der Größe von Schränken, von Bäumen und sogar so hoch, wie ein Hochhaus. Ihre Stengel und Hüte haben die verschiedensten Farben und Formen und du kannst Summen und Surren hören. Der Anblick ist atemberaubend.");
	if(wuerfel(6) > 3)
		textausgabe("Trotz dieser gleichzeitig verstörenden und beeindruckenden Schönheit dieses Ortes, wirst du dir schnell einer Sache bewußt: Wenn derart großes Leben hier unten möglich ist, dann wäre es vielleicht an der Zeit, ein wenig mehr Achtsamkeit und Wachsamkeit an den Tag zu legen.");
	auswahl("Du kannst die Pilzhöhle im Westen verlassen und einen Tunnel betreten (1) oder dich weiter in östlicher Richtung durch den Pilzwald bewegen (2) oder versuchen, ob du einen Weg in eine andere Richtung finden kansnt (3)", 3, ort87, ort147, ort210);
}

void ort147(void) {
	rotation++;
	raum = 147;
	textausgabe("Du befindest dich in einem Pilzwald. Überall um dich herum wachsen Pilze der verschiedensten Größe. Surrende Geräusche erfüllen die Luft.");
	if(wuerfel(6) > 4)
		ort210();
	auswahl("Du kannst dich weiter in westlicher Richtung bewegen (1) oder in östlicher Richtung (2). Wenn du etwas Zeit investierst, kannst du vielleicht noch einen anderen Weg entdecken (3)", 3, ort146, ort148, ort210);
}

void ort148(void) {
	rotation++;
	textausgabe("Du befindest dich in einem Pilzwald, tief unter der Erde. Um dich herum befindet sich ein bunter Wald aus Pilzen. Hier ist ein kleiner Teich voll klaren Wassers, ein paar Felsen laden zum darauf sitzen ein.");
	if(raum != 148)
		mahlzeit();
	raum = 148;
	auswahl("Du kannst dich weiter in westlicher Richtung bewegen (1) oder in östlicher (2). Wenn du etwas Zeit investierst, kannst du vielleicht noch einen anderen Weg entdecken (3)", 3, ort147, ort97, ort210);
}

void ort149(void) {
	rotation++;
	if(raum == 149)
		if(schluessel9 || schluessel66 || schluessel99 || schluessel111_2 || schluessel125 || schluesselbootshaus)
			textausgabe("Nach mehreren Versuchen gibst du auf. Es sieht nicht danach aus, das du den richtigen Schlüssel bei dir hast.");
		else
			textausgabe("Die Türe ist verschlossen und äußerst massiv. Ohne Schlüssel wirst du hier nicht weiterkommen.");
	else
		textausgabe("Der Stollen fällt langsam von Norden nach Süden ab. An dieser Stelle befindet sich ein etwa mannsgroße Türe mit schweren Eisenbeschlägen in der Ostwand. Ein paar Schienen laufen an der Ostwand entlang. Das Wurzelwerk breitet sich nur sehr spärlich an der Decke aus und entsprechend schlecht ist die natürliche Sicht hier unten.");
	raum = 149;
	if(wuerfel(6) > 3)
		textausgabe("Dir kommt es so vor, als würdest du Klopfgeräusche aus dem Stollen kommen hören.");
	if(wuerfel(6) > 5)
		textausgabe("Es kommt dir so vor, als hätte sich das Gestein des Felsens leicht verändert.");
	if(wuerfel(6) > 5)
		textausgabe("Du bist dir nicht sicher, aber trotz des schwachen Lichts hast du das Gefühl, kleine metallische Lichtreflexe auf der Felswand erkennen zu können.");
	if(wuerfel(6) > 4)
		textausgabe("Für einen kurzen Moment hattest du das Gefühl, ein Stöhnen von hinter der Türe gehört zu haben.");
	if(schluessel111_1)
		auswahl("Du kannst dem Tunnel nach Norden folgen (1) oder nach Süden (2) oder versuchen, ob einer deiner Schlüssel die Türe im Osten öffnet (3). Du kannst auch die Wände nach Geheimgängen absuchen (4).", 4, ort144, ort99, ort150, ort212, NULL, NULL);
	else if(schluessel9 || schluessel66 || schluessel99 || schluessel111_2 || schluessel125 || schluesselbootshaus)
		auswahl("Du kannst dem Tunnel nach Norden folgen (1) oder nach Süden (2) oder versuchen, ob einer deiner Schlüssel die Türe im Osten öffnet (3). Du kannst auch die Wände nach Geheimgängen absuchen (4).", 4, ort144, ort99, ort149, ort212, NULL, NULL);
	else
		auswahl("Du kannst dem Tunnel nach Norden folgen (1) oder nach Süden (2) oder versuchen, ob du die Türe im Osten öffnen kannst (3). Du kannst auch die Wände nach Geheimgängen absuchen (4).", 4, ort144, ort99, ort149, ort212, NULL, NULL);
}

void ort150(void) {
	rotation++;
	if((raum == 149) && (stollentroll == 150)) {
		charakter_t troll = { "Stollentroll", 9, 9, 10, 10 };
		if(!kampf(&spieler, &troll, 1, false, NULL))
            beenden(rot, EXIT_SUCCESS, "Die Fäuste des Stollentrolls hämmern gnadenlos auf deinen Kopf ein. Du spürst das Blut in deine Augen laufen, hörst das Knacken und Krachen deiner berstenden Schädelplatte. Ein heftiger Schmerz durchzuckt dich - dann verschwimmt alles vor deinen Augen, wird blasser, dunkel. Ein Tunnel ... da ist ein Licht am ENDE des Tunnels ...");
		// Dann platzieren wir mal die Zwerge um
		if(minenzwerge == 158)
			minenzwerge = 160;
	}
	textausgabe("Der Raum ist dunkel. Der Stollentroll, mit dem du eine wilde Keilerei hattest, liegt tot am Boden und sein Kadaver stinkt wie einst im Mai. Tatsächlich ist der Gestank so stechend, daß dir die Tränen in die Augen steigen. Im Westen befindet sich die offene Türe, durch die du hereingekommen bist, und im Süden glaubst du, einen weiteren Durchgang im Schatten ausmachen zu können.");
	raum = 150;
	auswahl("Du kannst nach Westen gehen (1) oder nach Süden (2) oder die Wände nach Geheimgängen absuchen (3)", 3, ort149, ort155, ort212);
}

void ort151(void) {
	rotation++;
	raum = 151;
	textausgabe("Der Raum, den du betrittst riecht, als wäre er aus Kuhfladen oder schlimmeren hergestellt. Die Wände erscheinen porös oder schlimmer zu sein. Jeder Schritt auf dem Boden läßt dich in dem braunen Schlick ein wenig einsinken.");
	auswahl("Du kannst dich nach Norden bewegen (1) oder dem stinkigen Weg weiter nach Osten folgen (2). Wenn es dich vor nichts graust, so kannst du auch die Wände nach Geheimgängen absuchen (3)", 3, ort87, ort152, ort212);
}

void ort152(void) {
	rotation++;
	if(raum == 79)
		textausgabe("Du bewegst dich durch einen stockdunklen Gang. Mehr als einmal triffst du auf abzweigende Gänge - und immer entscheidest du dich intuitiv, wo du langgehst. Schließlich wird es etwas heller. Deine Hände greifen durch eine Art von Ranken hindurch, du teilst sie und stehst Augenblicke später in einem hell erleuchteten Raum. Deine Augen tränen etwas und du benötigst einen Moment, um dich an das Licht zu gewöhnen. Offensichtlich war der Gang länger, als du dachtest.");
	raum = 152;
	textausgabe("Der Raum in dem du dich befindest, riecht eigenartig und überall an den Wänden hängen ranken eines seltsamen rötlichbraunen Grases herunter, so daß die darunterliegenden Wände eher zu erahnen als zu erkennen sind. Nur in westlicher Richtung kannst du einen Ausgang ausmachen, der Raum selber scheint eine Art totes Ende zu sein.");
	auswahl("Du kannst den Weg zurück nach Westen gehen (1) oder die Ranken absuchen, ob sich irgendwo darunter vielleicht noch ein weiterer Weg versteckt (2)", 2, ort151, ort79);
}

void ort153(void) {
	rotation++;
	raum = 153;
	textausgabe("Du befindest dich in einem unüberschaubar großen Wald aus Pilzen. Fremde Geräusche dringen von allen Seiten auf dich ein. Südlich von dir befindet sich der gigantische Pilz, der das Zentrum dieses Ortes zu sein scheint.");
	auswahl("Die Wand aus Pilzen läßt es offensichtlich nur zu, das du dich von hier aus in östlicher Richtung (1) oder weiter nach Süden auf den Riesenpilz zu bewegen kannst (2). Wenn du willst, kannst du etwas Zeit investieren und versuchen, ob du vielleicht noch einen weiteren Weg finden kannst (3)", 3, ort154, ort156, ort210);
}

void ort154(void) {
	rotation++;
	raum = 154;
	textausgabe("Du befindest dich inmitten eines Meeres aus Pilzen aller Größen und Farben. Die Luft wirkt hier sehr stickig und das Atmen fällt dir etwas schwerer als normal. Schweißperlen bilden sich auf deiner Stirn. Von hier aus führen Wege durch das Pilzdickicht in verschiedene Richtungen.");
        auswahl("Du könntest dich weiter in südlicher (1) oder westlicher (2) Richtung an den Riesenpilz im Zentrum heranwagen oder dich in nördlicher (3) Richtung von ihm entfernen. Wenn du etwas Zeit zu investieren bereit bist, kannst du auch nach weiteren Wegen im Dickicht suchen (4).", 4, ort157, ort153, ort97, ort210);
}

void ort155(void) {
	rotation++;
	if(raum == 159) {
		textausgabe("Der Raum liegt im Dunkeln. Du glaubst vor dir im Schatten eine Bewegung auszumachen. Ein Brüllen ist zu hören - und plötzlich, stürzt vor dir die Decke ein! Du nimmst die Beine in die Hand und rennst wie von der Tarantel gestochen zur Türe, durch die du gerade erst hereingekommen bist, hinaus.");
		raum = 155;
		ort159();
	}
	else {
		textausgabe("Der Raum liegt im Dunkeln. Du kannst über dir im Gestein ein schweres Knacken und Knirschen hören. Eine Gänsehaut jagt dir über den Rücken. Wenn du etwas nicht möchtest, dann an diesem Ort unter Stein begraben zu werden. Hastig drehst du dich um und rennst zurück nach Norden.");
		raum = 155;
		ort150();
	}
}

void ort156(void) {
	rotation++;
	if(raum == 156) {
		textausgabe("Trotz deiner intensiven Suche, kannst du keinerlei weitere Wege entdecken.");
		mahlzeit();
	}
	raum = 156;
	textausgabe("Du befindest dich jetzt im Licht des Riesenpilzes. Je näher du dem Stamm kommst, um so gewaltiger empfindest du ihn. Die Ausmaße sind so riesig, das er an der Oberfläche bestimmt jeden einzelnen der Wolkenkratzer im durch Betonstahl verseuchten Herzen Frankfurts überragen dürfte. Der freie Raum vor dem Riesenstamm ist nicht sehr groß, es wirkt, als würden sich alle anderen Pilze in diesem für deine Verhältnisse gewöhnungsbedürftigen Wald, sich auf diesen Pilz zubewegen.");
	if(wuerfel(6) > 4)
		textausgabe("Eine frische Brise kommt auf und es riecht jetzt salzig in der Luft. Du glaubst sogar, das Schlagen von Wellen an das Ufer hören zu können.");
	auswahl("Du kannst dich von hier aus in südlicher Richtung (1) oder in nördlicher Richtung (2) bewegen. Wenn du etwas Zeit investierst, kannst du auch den dichten Pilzwald nach weiteren Wegen durchsuchen (3)", 3, ort163, ort153, ort210);
}

void ort157(void) {
	rotation++;
	raum = 157;
	textausgabe("Du machst einen weiten Bogen um den Zentralpilz herum. Weit im Osten glaubst du, eine Wand sehen zu können, aber aufgrund des Dunstes in der Luft könnte es auch eine reine Wunschprojektion deines Geistes sein. Der Pilzstamm liegt von dir aus weit im Westen. Die Pilze um dich herum verbreiten einen üblen Gestank, trotzdem kannst du atmen.");
	auswahl("Du kannst dich von hier aus weiter in südlicher Richtung (1) oder in nördlicher Richtung (2) bewegen. Wenn du etwas Zeit investierst, vermagst du vielleicht noch weitere Wecke in diesem erdrückenden Pilzwald auszumachen (3)", 3, ort164, ort154, ort210);
}

void ort158(void) {
	charakter_t zwerg[3] = { { "Zwerg mit Spitzhacke", 7, 7, 9, 9 }, { "Zwerg mit Schaufel", 5, 5, 6, 6}, { "Zwerg mit Hammer", 8, 8, 7, 7} };
	rotation++;
	raum = 158;
	textausgabe("Schon als du dich dem Raum näherst, bemerkst du die Veränderung der Helligkeit. Als du ihn schließlich betrittst, ist er taghell erleuchtet. Eine Art von Käfern krabbeln hier auf den Wänden entlang, ihre Panzer schimmern in hellem weißlichen Licht.");
	if(minenzwerge == 158) {
		if(schluessel111_1 && (wuerfel(6) > 4)) {
			textausgabe("Die Zwerge beäugen dich äußerst mißtrauisch.\n\"Du weißt nicht zufällig, wo mein Schlüssel abgeblieben ist, Fremder, hm?\" fragt er feindselig, während ein anderer Zwerg um dich herumgeht und versucht, deinen Rucksack zu öffnen.\n\"Wie ich's mir gedacht habe!\" brüllt er triumphierend, \"Der Mensch ist ein lausiger Dieb!\"\nDu siehst, wie sie ihre Schaufeln und Spitzhacken in der Haltung verlagern. Jetzt sind es keine Werkzeuge mehr, sondern Waffen.");
			if(!kampf(&spieler, zwerg, 1, false, ort99))
                beenden(rot, EXIT_SUCCESS, "Drei Zwerge gegen sich aufzubringen war wirklich eine dumme Idee. Du sinkst mit einer riesigen Schädelwunde am Hinterkopf zusammen. Du siehst nichts mehr, hörst aber das Näherkommen ihrer Schritte. In Gedanken bist du auf dem Mönchengladbacher Hauptfriedhof. Du kniest nieder am Grab deiner Großeltern und entspannst dich. DU erträgst den Schmerz der Schläge - und dann verebbt der Schmerz ganz. Du hörst nichts mehr. Du siehst nichts mehr. Du gleitest hinab in die Schwärze. Dein ENDE ist gekommen.");
			// Aus ist's mit den Zwergen
			minenzwerge = 0;
		}
		if(!schluessel111_1) {
			textausgabe("Du kannst sehen, das an der Hose eines der Zwerge ein Schlüssel an einem Lederbändchen herabbaumelt.");
			if(janeinfrage("Möchtest du versuchen, dem Zwerg den Schlüssel heimlich und in einem unbeobachteten Moment zu stibitzen (j/n)?")) {
				if(tdg(&spieler)) {
					schluessel111_1 = true;
					textausgabe("Gerade, als keiner der Zwerge in deine Richtung schaut, hältst du für einen Moment die Luft an und greifst zu. Es ist ganz leicht, den Knoten zu lösen - und schon hältst du den Schlüssel in der Hand. Mit einer sanft gleitenden Bewegung läßt du ihn in deiner Tasche verschwinden.");
				}
				else {
					if(!kampf(&spieler, zwerg, 1, false, ort99))
                        beenden(rot, EXIT_SUCCESS, "Drei Zwerge gegen sich aufzubringen war wirklich eine dumme Idee. Du sinkst mit einer riesigen Schädelwunde am Hinterkopf zusammen. Du siehst nichts mehr, hörst aber das Näherkommen ihrer Schritte. In Gedanken bist du auf dem Mönchengladbacher Hauptfriedhof. Du kniest nieder am Grab deiner Großeltern und entspannst dich. DU erträgst den Schmerz der Schläge - und dann verebbt der Schmerz ganz. Du hörst nichts mehr. Du siehst nichts mehr. Du gleitest hinab in die Schwärze. Dein ENDE ist gekommen.");
					// Aus ist's mit den Zwergen
					minenzwerge = 0;
				}
			}
			if(minenzwerge == 158)
				textausgabe("Die Zwerge sind viel zu abgelenkt, um dich tatsächlich wahrzunehmen. Du siehst, wie sie den Felsen scheinbar streicheln und mit ihren Spitzhacken dann gezielt auf Stellen schlagen. Der Fels teilt sich und legt darunterliegende Gesteinsadern frei. Der Vorgang wirkt viel natürlicher und bewußter, als die plumpe Art der Menschen, wahllos auf den Fels einzuprügeln und ihn Kubikmeterweise auszuplündern. Sie entnehmen immer nur Teile der Gesteine aus der Ader, so als wollten sie verhindern, die Ader ausbluten zu lassen. Die so erwirtschafteten Steine legen sie beinahe zärtlich in die Lore, die hier auf den Schienen steht.");
		}
	}
	auswahl("Du kannst dem Tunnel nach Norden folgen (1) oder nach Osten (2) oder die Wände nach Geheimgängen absuchen (3)", 3, ort99, ort159, ort212);
}

void ort159(void) {
	rotation++;
	if((raum == 155) && ((minenzwerge == 158) || (minenzwerge == 160))) {
		minenzwerge = 159;
		charakter_t zwerg[3] = { { "Zwerg mit Spitzhacke", 7, 7, 9, 9 }, { "Zwerg mit Schaufel", 5, 5, 6, 6}, { "Zwerg mit Hammer", 8, 8, 7, 7} };
		textausgabe("Die Zwerge sind herangestürmt und beäugen dich äußerst mißtrauisch.\n\"Du weißt nicht zufällig, wo mein Schlüssel abgeblieben ist, Fremder, hm?\" fragt einer von ihnen feindselig, während ein anderer Zwerg um dich herumgeht und versucht, deinen Rucksack zu öffnen.\n\"Wie ich's mir gedacht habe!\" brüllt er triumphierend, \"Der Mensch ist ein lausiger Dieb!\"\nDu siehst, wie sie ihre Schaufeln und Spitzhacken in der Haltung verlagern. Jetzt sind es keine Werkzeuge mehr, sondern Waffen.");
		if(!kampf(&spieler, zwerg, 1, false, ort158))
            beenden(rot, EXIT_SUCCESS, "Drei Zwerge gegen sich aufzubringen war wirklich eine dumme Idee. Du sinkst mit einer riesigen Schädelwunde am Hinterkopf zusammen. Du siehst nichts mehr, hörst aber das Näherkommen ihrer Schritte. In Gedanken bist du auf dem Mönchengladbacher Hauptfriedhof. Du kniest nieder am Grab deiner Großeltern und entspannst dich. DU erträgst den Schmerz der Schläge - und dann verebbt der Schmerz ganz. Du hörst nichts mehr. Du siehst nichts mehr. Du gleitest hinab in die Schwärze. Dein ENDE ist gekommen.");
		// Aus ist's mit den Zwergen
		minenzwerge = 0;
	}
	if(raum == 159)
		if(schluessel9 || schluessel66 || schluessel99 || schluessel111_1 || schluessel125 || schluesselbootshaus)
			textausgabe("Nach mehreren Versuchen gibst du auf. Es sieht nicht danach aus, das du den richtigen Schlüssel bei dir hast.");
		else
			textausgabe("Die Türe ist verschlossen und äußerst massiv. Ohne Schlüssel wirst du hier nicht weiterkommen.");
	else
		textausgabe("Der Stollen fällt hier stärker von Osten nach Westen ab. Das Wurzelwerk breitet sich nur sehr spärlich an der Decke aus und entsprechend schlecht ist die natürliche Sicht hier unten. Die Struktur der Wände an der Südwand ist teilweise zerklüftet, scharfkantige Strukturen stehen hervor und der Schattenwurf ist großflächig.");
	raum = 159;
	if(wuerfel(6) > 3)
		textausgabe("Dir kommt es so vor, als würdest du Klopfgeräusche aus dem Stollen kommen hören.");
	if(wuerfel(6) > 3)
		textausgabe("Es kommt dir so vor, als hätte sich das Gestein des Felsens leicht verändert.");
	if(wuerfel(6) > 4)
		textausgabe("Du bist dir nicht sicher, aber trotz des schwachen Lichts hast du das Gefühl, kleine metallische Lichtreflexe auf der Felswand erkennen zu können.");
	if(schluessel111_2)
		auswahl("Du kannst dem Tunnel nach Westen folgen (1) oder nach Osten (2) oder versuchen, ob einer deiner Schlüssel die Türe in der Nordwand öffnet (3). Du kannst auch die Wände nach Geheimgängen absuchen (4).", 4, ort158, ort160, ort155, ort212, NULL, NULL);
	else if(schluessel9 || schluessel66 || schluessel99 || schluessel111_1 || schluessel125 || schluesselbootshaus)
		auswahl("Du kannst dem Tunnel nach Westen folgen (1) oder nach Osten (2) oder versuchen, ob einer deiner Schlüssel die Türe in der Nordwand öffnet (3). Du kannst auch die Wände nach Geheimgängen absuchen (4).", 4, ort158, ort160, ort159, ort212, NULL, NULL);
	else
		auswahl("Du kannst dem Tunnel nach Westen folgen (1) oder nach Osten (2) oder die Wände nach Geheimgängen absuchen (3)", 4, ort158, ort160, ort212);
}

void ort160(void) {
	rotation++;
	raum = 160;
	textausgabe("Der Tunnel endet in einem großen Raum, der sehr stark nach Brandwein und altem Stinkekäse riecht. Er ist vollgestellt mit Schaufeln, Spitzhacken und allerlei anderem Gerümpel.");
	if(!schluessel111_2)
		if(janeinfrage("An einem Haken neben der Eingangstüre hängt ein Kupferschlüssel. Deutlich kannst du darauf die Schriftzeichen \"111\" ausmachen. Möchtest du den Schlüssel an dich nehmen (j/n)?"))
			schluessel111_2 = true;
	if((minenzwerge == 160) && (schluessel111_1 || schluessel111_2)) {
		charakter_t zwerg[3] = { { "Zwerg mit Spitzhacke", 7, 7, 9, 9 }, { "Zwerg mit Schaufel", 5, 5, 6, 6}, { "Zwerg mit Hammer", 8, 8, 7, 7} };
		textausgabe("Erst spät bemerkst du die Zwerge, die zwischen den Gerätschaften im Schatten stehen.\n\"Du weißt nicht zufällig, wo mein Schlüssel abgeblieben ist, Fremder, hm?\" fragt einer von ihnen feindselig, während ein anderer Zwerg um dich herumgeht und versucht, deinen Rucksack zu öffnen.\n\"Wie ich's mir gedacht habe!\" brüllt er triumphierend, \"Der Mensch ist ein lausiger Dieb!\"\nDu siehst, wie sie ihre Schaufeln und Spitzhacken in der Haltung verlagern. Jetzt sind es keine Werkzeuge mehr, sondern Waffen.");
		if(!kampf(&spieler, zwerg, 1, false, ort159))
            beenden(rot, EXIT_SUCCESS, "Drei Zwerge gegen sich aufzubringen war wirklich eine dumme Idee. Du sinkst mit einer riesigen Schädelwunde am Hinterkopf zusammen. Du siehst nichts mehr, hörst aber das Näherkommen ihrer Schritte. In Gedanken bist du auf dem Mönchengladbacher Hauptfriedhof. Du kniest nieder am Grab deiner Großeltern und entspannst dich. DU erträgst den Schmerz der Schläge - und dann verebbt der Schmerz ganz. Du hörst nichts mehr. Du siehst nichts mehr. Du gleitest hinab in die Schwärze. Dein ENDE ist gekommen.");
		// Aus ist's mit den Zwergen
		minenzwerge = 0;
	}
	// Der Mechanismus für den Durchgang zwischen Raum 90 und Raum 91
	if(wuerfel(6) > 3)
		if((gitteroffen == false) && janeinfrage("In einem leeren Brandweinfaß bemerkst du einen Hebel. Als du daran ziehst merkst du, daß er nicht zufällig darin ist. Es scheint eine Art Tarnung für den Hebel zu sein. Möchtest du kräftig an dem Hebel ziehen (j/n)?"))
			gitteroffen = true;
	auswahl("Du kannst den Tunnel zurück nach Westen gehen (1) oder die Wände nach Geheimgängen absuchen (2)", 2, ort159, ort212);
}

void ort161(void) {
	rotation++;
	if(raum != 161)
		textausgabe("Du stehst in einer von leuchtenden Adern durchzogenen Höhle, die voller Gold und Juwelen ist. Ein Drachenhort, ganz so, wie man ihn aus den Märchen und Erzählungen der Großväter kennt. Das kleine Kind in dir jauchzt fröhlich vor Vergnügen an dem Anblick - und das Herz des Obdachlosen würde sich am liebsten alles, was es hier sieht, einstecken und zu Geld machen, wenn, ja wenn Geld noch irgendeine Bedeutung hätte. Schmerzlich wirst du dir bewußt, daß du in der Tat ein Flüchtling bist, ein Flüchtling, der vor einer unbekannten Besatzungsmacht flieht, die seine Heimatstadt mit der Hilfe seltsamer Maschinenwesen in eine Landschaft aus rauchenden Trümmern und Asche verwandelt hat.\nDu beginnst dem Berg aus Edelmetallen zu durchsuchen, nach etwas sinnvollem, etwas nützlichem. Eine Waffe wäre vielleicht interessant, aber alles, was du hier findest, sind scheinbar wertvolle Gesteinsbrocken, Juwelen, aber nichts, was dir wirklich weiter helfen würde.");
	raum = 161;
	if(wuerfel(6) > 4)
		textausgabe("In all den Kostbarkeiten, stößt du auf einen Zettel. Du entfaltest ihn und liest nur ein einziges Wort: \"Handschuh\"");
	if(wuerfel(6) > 4)
		textausgabe("Während du dich am Fuße des Horts entlangbewegst, bemerkst du ein mit Kreide an die Wand geschriebenes Wort: \"Bett\"");
	if(wuerfel(6) > 4)
		textausgabe("In einer Ecke hat jemand das Wort \"Brezel\" auf den Boden geritzt.");
	if(wuerfel(6) > 4)
		textausgabe("Am Ausgang des Horts hat jemand das Wort \"Zeit\" in Augenhöhe an die Wand geschrieben.");
	auswahl("Du kannst den Hort verlassen und zurück nach Osten gehen (1) oder die Wände absuchen, ob nicht ein Geheimgang an diesen Ort führt (2)", 2, ort98, ort212);
}

void ort162(void) {
	rotation++;
	raum = 162;
	textausgabe("Du befindest dich in einem breiten Tunnel. Im Osten siehst du ein Licht, vermutlich ist es der Ausgang aus diesem Tunnel, während es in westlicher Richtung nur tiefer in die Schwärze hineinzugehen scheint.");
	auswahl("Du kannst dem Tunnel nach Westen folgen (1) oder nach Osten in Richtung auf das Licht zu gehen (2). Wenn du magst, kannst du auch die Wände nach Geheimgängen absuchen (3)", 3, ort98, ort163, ort212);
}

void ort163(void) {
	rotation++;
	if(raum == 163)
		textausgabe("Du gehst an den endlosen Reihen der Stämme entlang, hast jedoch Angst den einzigen Weg, den du durch den Pilzwald kennst, aus den Augen zu verlieren, so drehst du um und näherst dich wieder der Felswand im Westen.");
	raum = 163;
	textausgabe("Du befindest dich an einem großen, weiten Strand, an dessen Ufer die Wellen eines Meeres branden. Weit nördlich von hier aus befindet sich ein riesiger Pilz und ein Pilzwald zieht sich am Strand entlang, soweit dein Auge reicht. Nicht weit im Westen von dir ist die Felswand die sich weit nach oben streckt. Das Meer erstreckt sich bis zum Horizont, du kannst seine Ausmaße nicht einmal erahnen. Salzig duftende, frische Brisen streichen dir durch das Gesicht. In der Westwand befindet sich ein Tunnel.");
	auswahl("Du kannst dich nach Westen am Strand lang bewegen und den Tunnel im Berg betreten (1) oder du kannst dich in Richtung des Riesenpilzes nach Norden weiterbewegen (2). Wenn du willst, kannst du den Strand auch nach weiteren Wegen absuchen (3)", 3, ort162, ort156, ort210);
}

void ort164(void) {
	int zufallszahl = wuerfel(1000);
	int geraten;
	char eingabe[21];
	rotation++;
	if((raum == 164) && schluessel9)
		switch(wuerfel(6)) {
			case 1: textausgabe("\"Du schon wieder?\" lächelt der kleine Gnom dich an, dann schließt er die Augen und ignoriert dich.");
				if(wuerfel(6) > 4)
					textausgabe("\"Der Tempel birgt ein Geheimnis!\" lächelt der kleine Blauhäutige und wendet sich ab.");
				break;
			case 2: textausgabe("\"Hat dir mein kleines Spielchen gefallen?\" fragt der Gnom und lächelt dich desinteressiert an.");
				break;
			case 3: textausgabe("Der kleine Gnom reagiert nicht auf dich, egal wie oft du ihn ansprichst.");
				if(wuerfel(6) > 4)
					textausgabe("\"Laß ab von Drachen!\" lächelt der kleine Gnom und ignoriert dich und deine Fragen auch weiterhin.");
				break;
			case 4: textausgabe("\"Mich nennen alle nur Papa!\" grinst der kleine Gnom, zieht an seiner Pfeife und bläst ein Rauchwölkchen aus. Es ist ein sehr kunstvolles Rauchwölkchen, das davonschwebt. Dir kommt es so vor, als würde sie eine Kutsche mit 4 Pferden davor darstellen.");
				break;
			case 5: textausgabe("\"Wie ist das Wetter da oben?\" fragt der kleine Blaue verschmitzt.");
				if(wuerfel(6) > 4)
					textausgabe("\"Manchmal bleibt der Drache Sieger!\" murmelt das Männlein und bläst einen atemberaubend großen Rauchkringel in die Luft.");
				break;
			default: textausgabe("\"Hast du nicht noch etwas anderes zu tun?\" fragt das kleine Männlein verschmitzt und ignoriert all deine Worte.");
				 if(wuerfel(6) > 4)
					 textausgabe("\"Hab' Angst vor Drachen!\" sagt das Männlein weise und wendet sich von dir ab.");
				 break;
		}
	if(raum == 157)
		textausgabe("Als du aus dem Dickicht der Pilzstämme trittst, siehst du ein seltsames Männlein, das gemütlich eine Pfeife rauchend im Schatten eines Pilzhauses sitzt. Das Männlein trägt rotes Beinkleid und eine entsprechende Zipfelmütze, und hat eine bläuliche Haut. Ein weißer Rauschebart ziert sein freundliches Gesicht mit der großen, knubbeligen Nase.");
	raum = 164;
	if(!schluessel9 && janeinfrage("Das kleine Männlein sagt: \"Ein Rätsel habe ich für dich, löst du es, dann lohnt es sich! Möchtest du, daß ich dir mein Rätsel stelle (j/n)?\"")) {
		textausgabe("Der kleine Gnom lächelt dich gutmütig an.\n\"Ich habe mir eine Zahl zwischen 1 und 1000 ausgedacht. Du hast 9 Versuche, die Zahl zu erraten.\"");
		for(int i=1; i <= 9; ++i) {
            vordergrundfarbe(gelb);
			textausgabe("\"Was denkst du, wie lautet meine Zahl?\"");
            vordergrundfarbe(magenta);
            texteingabe(eingabe, 20);
            vordergrundfarbe(weiss);
			geraten = atoi(eingabe);
			if(zufallszahl == geraten) {
				schluessel9 = true;
				textausgabe("\"Du bist sehr klug!\" lobt dich der kleine Gnome und gibt dir einen kleinen, blauen Schlüssel, der wie die Zahl 9 oder 6 geformt ist.");
				break;
			}
			if(zufallszahl < geraten)
				textausgabe("\"Deine Zahl ist zu groß!\" lächelt der kleine Gnom.");
			if(zufallszahl > geraten)
				textausgabe("\"Deine Zahl ist zu klein!\" schmunzelt der rauschebärtige Gnom.");
		}
	}
	textausgabe("Der kleine Gnom lehnt sich zurück und zieht schmauchend an seiner Pfeife.");
	auswahl("Du kannst den Weg zurück in nördlicher Richtung nehmen (1) oder suchen, ob du noch einen weiteren Ausgang finden kannst (2). Vielleicht möchtest du ja auch noch einen Schwatz mit dem kleinen Gnom halten (3)?", 3, ort157, ort210, ort164);
}

// ============================================
// Dwellmerdan - das Dorf der Dwellmer (Zwerge)
// ============================================
void ort165(void) {
	// Der Eingang zum Zwergendorf
	if(wuerfel(6) > 4)
		textausgabe("Du befindest dich an der großen Eingangsschleuse des Zwergendorfes. Die Schleuse ist so groß, das man in ihr bequem einen Zug mit fünf Waggons parken könnte - und hätte trotzdem noch genügend Platz.");
	textausgabe("Du kannst den Wächtern in der Schleusensteuerung von Dwellmerdan ein Zeichen geben, ob du das Dorf der Dwellmer betreten möchtest, oder ob du hinaus in die Höhlen der Jagd willst.\nZum Betreten der Dwellmersiedlung mußt du \"Melone\" in den großen Kupfertrichter in der Mitte der Schleuse sagen, um hingegen die Höhlen der Jagd zu betreten heißt das Losungswort \"Fellohne\".");
	raum = 165;
	auswahl("Sagst du \"Melone\" (1) oder \"Fellohne\" (2)? in den großen Kupfertrichter?", 2, ort166, ort142);
}

void ort166(void) {
	// Der große Platz
	raum = 166;
	textausgabe("Du befindest dich auf dem \"Großen Platz\", obwohl die Bezeichnung \"der einzige Platz des Dorfes\" genauso treffend wäre - und verglichen mit der Höhle des Riesenpilzes ist der große Platz und das in die Felsen gehauene Dorf der Dwellmer nun wirklich nicht der Rede wert. Wäre es das erste gewesen, was du nach deiner Rutschpartie in die unterirdische Welt gesehen hättest, wärst du vermutlich vor Ergriffenheit erzittert, aber in den letzten Tagen hast du einfach schon zuviel erlebt, zuviel gesehen und auch zuviele Schmerzen und Verletzungen erlitten. Der Platz ist über und über mit Mustern versehen, er erinnert ein wenig an die Zengärten Japans, wobei hier jedoch weniger das Bild der Leere, als vielmehr die Erzählkunst der Dwellmer in den Vordergrund gerückt wird.");
	if(wuerfel(6) > 4)
		textausgabe("Vom Großen Platz aus kannst du all die Zentralorte des Dorfes erreichen, wie etwa das Haus Haus der Feier, wo die Dwellmer es gerne ordentlich krachen lassen, oder der Arena, wo die Zwerge sich schon gegenseitig eines auf die Fünf geben.");
	auswahl("Möchtest du das Dorf der Dwellmer verlassen und die Höhlen der Jagd betreten (1), ins Haus der Feier (2), ins Haus der Maschinen (3), in das Haus der Helden der Waffen (4), in das Haus der herzhaften Düfte (5), in die Arena des kochenden Blutes (6), in das Haus der Einkehr (7), in das Haus der Ältesten (8), in das Haus der Alchemiker (9) oder in das Haus der Ruhe (10). Du kannst auch einfach nur auf dem Platz herumstehen und die Zeit totschlagen (11).", 11, ort142, ort167, ort168, ort169, ort170, ort171, ort172, ort173, ort174, ort175, ort166);
}

void ort167(void) {
	// Das Haus der Feier
}

void ort168(void) {
	// Das Haus der Maschinen
}

void ort169(void) {
	// Das Haus des Helden der Waffen = Held = Bewahrer
}

void ort170(void) {
	// Das Haus der herzhaften Düfte
}

void ort171(void) {
	// Die Arena des kochenden Blutes
}

void ort172(void) {
	// Das Haus der Einkehr
}

void ort173(void) {
	// Das Haus des Ältesten
}

void ort174(void) {
	// Das Haus der Alchemiker
}

void ort175(void) {
	// Das Haus der Ruhe
	int n;
	if(raum == 142) { // Du erwachst nach dem Niederschlag
		textausgabe("Als du deine Augen aufschlägst, hast du das Gefühl, dein ganzer Kopf wäre in Watte gepackt. Das Bild will sich nicht scharf stellen vor deinen Augen, ja, du hast sogar das Gefühl, als würde das Bild in verschiedene Richtungen fallen, eines für das rechte und eines für das linke Auge. Du versuchst etwas zu murmeln und nickst wieder ein.\nBeim nächsten Mal, als du die Augen öffnest, ist das Bild klarer. Vor dir siehst du eine Gestalt. Offensichtlich ist ihr Kopf nah vor deinem, aber obwohl sie den Mund bewegt, hörst du kein Wort - und auch das Gefühl, den Kopf in Zuckerwatte gebettet zu haben ist noch da. Du beschließt zu antworten, schließt die Augen um etwas Kraft zu sammeln - und bist schon wieder eingeschlafen.\nNachdem du die Augen wieder aufgeschlagen hast, stellst du fest, daß dein Kopf sich nicht mehr wie in Watte gebettet anfühlt. Dafür spürst du jetzt einen scharfen Schmerz, der sich durch deine ganze Wirbelsäule erstreckt, als du auch nur den Versuch machst, den Kopf einen Millimeter weit zu drehen. Offensichtlich liegst du in einem Bett, einem sehr angenehmen Bett. Es fühlt sich bequem an - und trotzdem scheint es deinen Körper zu umschließen, zu fixieren. In deinen Augenwinkeln erkennst du zwei Gestalten, die sich miteinander zu unterhalten scheinen. Du strengst dich an - und kannst Worte verstehen, Worte, deren Klang dir falsch erscheint. Die Rhytmik ist irgendwie atonal - und dennoch kannst du der Unterhaltung folgen.\n\"Siehst du, siehst du! Das Kerlchen hat die Augen offen. Ich wette 5 zu 1, das er in den nächsten 2 Minuten wieder wegdöst!\" ertönt eine keckernde Stimme, während eine etwas tiefere ihr antwortet: \"Die Wette hast du schon jetzt verloren!\"\nEines der beiden Gesichter kommt jetzt näher, füllt einen Großteil deines Gesichtsfeldes aus.\n\"Bist du wach?\" Die Stimme die du hörst ist die tiefere. Das Gesicht ist haarig, nein, behaart. Die Person hat ein irgendwie grobschlächtiges und doch freundliches bärtiges Gesicht.\nDu öffnest deinen Mund, willst antworten, aber dein Mund ist zu trocken - und so entströmt nur ein Krächzen deinen Stimmbändern. Sehr schnell ist jetzt auch die zweite Person bei dir, sie hat eine Ampulle in der Hand, aus der eine Art Glasstrohhalm hervorschaut. Sie zielt mit dem offenen Stück in deinen Mund und spritzt Wasser hinein. Das Schlucken tut weh, trotzdem bist du dankbar, bringt das Nass doch eine angenehme Kühle auf deine Stimmbänder. Das zweite Wesen hat keinen Rauschebart wie das erste, nur einen sanften Bartflaum auf den Wangen.\n\"Trink!\" flüstert dir das Wesen jetzt mit einer angenehmeren, höhren Tonlage zu. Dir gelingt es, den Mund um den Strohhalm zu schließen und daran zu saugen. Das Brennen im Hals wird mit jedem Mal weniger - und ein wohliges Gefühl breitet sich in dir aus. Du beschließt dich zu bedanken und lallst: \"Dampfe!\"\nDann sackt das Bild vor deinen Augen weg.\nDu weißt nicht, wieviel Zeit vergangen ist, als du das nächste Mal aufwachst, aber diesmal fühlst du dich quietschlebendig. Du richtest dich auf uns sitzt im Bett. Das Wesen, augenscheinlich eine Art von Zwerg, wie diejenigen, die du schon in den Minen gesehen hast, hört auf mit ihrer Arbeit an einem Schreibtisch und steht auf, dann kommt es zu dir hinüber.\n\"Du siehst besser aus!\" sagt es zu dir.\n\"Ich fühle mich auch schon viel besser!\" antwortest du mit klarer Stimme.\n\"Das höre ich gerne\" antwortet das Wesen und tritt näher. \"Und jetzt streck doch bitte einmal deine Zunge heraus.\"\nKeine 5 Minuten später hast du eine vollständig medizinische Untersuchung hinter dir, die erste seit Jahren! Und daß das Wesen ein Arzt ist, daran hast du jetzt auch keinen Zweifel mehr, auch wenn der fehlende weiße Kittel und kein Stethoskop um den Hals doch etwas verwirrend wirken.\n\"Würdest du das bitte trinken?\" fragt dich das Wesen und reicht dir eine kleine graue Kristallflasche mit einer bräunlich aussehenden Brühe darin.");
		if(janeinfrage("Willst du die Kristallflasche annehmen und der Bitte des Arztes nachkommen, sie zu trinken (j/n)?")) {
			n = wuerfel(6);
			staerkesteigerung(n, n);
			n = wuerfel(6);
			gewandheitssteigerung(n, n);
			n = wuerfel(6);
			glueckssteigerung(n, n);
		}
		else
			textausgabe("\"Wie du meinst!\" entgegnet das Wesen, und stellt die kleine Flasche in einen Schrank zurück.");
		textausgabe("\"Mein Name ist Arianna.\", stellt sich das Wesen vor, \"Ich bin die Heilkundige dieses Dorfs.\"\nHeilkundige? Du hättest schwören können, einen Mann vor dir zu haben. Deine Verwunderung versuchst du zu verbergen, schluckst einmal kurz und nennst ihr deinen Namen. Im Laufe der nächsten halben Stunde erklärt dir Arianna, was passiert ist in der Zeit, während du geschlafen hast - und vor allem, WARUM du geschlafen hast. Und das kam so:\nDie fünf Schlüssel, die du zusammengesucht hast, sind so etwas wie ein Notbehelf, falls die Magie des Tores einmal versagen sollte. Mit Hilfe dieser 5 Schlüssel kann man das Tor manuell öffnen, man muß nur im nächsten Augenblick zur Seite springen, da es auch eine Falle aktiviert, um ungebetene Gäste abzuhalten - und eben jene Falle hast du aktiviert. Dein Metabolismus war glücklicherweise kräftig genug, daß du die Falle überlebt hast, wenn auch zu dem Preis, daß du querschnittgelähmt warst, ein Problem, das für die Heilkundige dieses Ortes jedoch nur eine Lapalie ist. Wie es scheint haben Dwellmer, denn so nennen sich die Zwerge selber, bei ihren Tätigkeiten selber öfter einmal den Hang dazu, ihren Körper unnötig zu überlasten - und die Heilkünste Ariannas zu benötigen.\nJetzt, wo du wach bist, kannst du dich einkleiden, neu einkleiden. Eine andere Dwellmerfrau war scheinbar so nett - und hat dir neue Kleidung geschneidert. Deine alte war etwas mitgenommen, durch die Jahre, aber auch durch die Erlebnisse der letzten Tage. Die neue Kleidung fühlt sich sehr leicht an und paßt wie angegossen, wobei ihr Stoff eine Farbe irgendwo zwischen Perl- und Cremefarben darstellt.\nAls du fertig angekleidet bist, betrachtest du dich im Spiegel. Dein Bart ist etwas gewachsen, und deine Haare sind lang, aber im Nacken zu einem Zopf zusammengeflochten, ganz wie es hier die Mode bei den Herren zu sein scheint.");
	}
	if(raum != 175)
		textausgabe("Du befindest dich im \"Haus der Ruhe\", einer Art Haus der Erholung im Dorf der Dwellmer. Hier geht man hin, wenn das eigene Wohlbefinden angegriffen ist - oder man einfach nur Ruhe vor den Pflichten des Alltags oder den anderen Dwellmern benötigt.");
	if(raum == 175) {
		if(wuerfel(6) > 5) {
			textausgabe("Du suchst einen der Ruheräume auf, wo du dich entkleidest und die Sauna betrittst. Als du lässig an der Wand lehnst, betritt Arianna den Raum, nur ein Handtuch lässig über die Schulter geworfen. Sie begrüßt dich und fragt, ob sie sich dem Saunagang anschließen darf.");
		       if((arianna >= 40) || janeinfrage("Bist du dazu bereit, den Saunagang mit Arianna gemeinsam zu machen(j/n)?")) {
				       textausgabe("Du hast nichts dagegen und ihr verbringt die nächste Stunde mit einer lockeren Plauderei und noch mehr Schwitzen. Nach dem Saunagang erhältst du von ihr sogar noch eine Ganzkörpermassage. Du fühlst dich so wohl, wie noch nie in deinem Leben.");
				       zweisamkeit(1);
			} else {
				textausgabe("Du verneinst, da du dich zu erschöpft fühlst. Sie nickt verstehend und verläßt den Saunaraum. Es ist ja nicht so, als wäre dies die einzige Sauna im Haus der Ruhe.");
				zweisamkeit(-1); 
			}
		} else 
			textausgabe("Du suchst einen der Ruheräume auf, wo du zuerst einen Saunagang machst und dich anschließend auf einem Massagesessel ausruhst. Als du einige Stunden später wieder aufwachst, fühlst du dich vollkommen erfrischt.");
		spieler.staerke = spieler.staerke_start;
		spieler.gewandheit = spieler.gewandheit_start;
		spieler.glueck = spieler.glueck_start;
	}
	raum = 175;
	auswahl("Du kannst hier eine Verschnaufpause einlegen (1) oder das Haus der Ruhe verlassen (2).", 2, ort175, ort166);
}

void ort176(void) {
}

void ort177(void) {
}

void ort178(void) {
}

void ort179(void) {
}

void ort180(void) {
}

void ort181(void) {
}

void ort182(void) {
}

void ort183(void) {
}

void ort184(void) {
}

void ort185(void) {
}

void ort186(void) {
}

void ort187(void) {
}

void ort188(void) {
}

void ort189(void) {
}

void ort190(void) {
}

void ort191(void) {
}

void ort192(void) {
}

void ort193(void) {
}

void ort194(void) {
}

void ort195(void) {
}

void ort196(void) {
}

void ort197(void) {
}

void ort198(void) {
}

void ort199(void) {
}

// -----------
// --- 200 ---
// -----------

void ort200(void) {
}

void ort201(void) {
}

void ort202(void) {
}

void ort203(void) {
}

void ort204(void) {
}

void ort205(void) {
}

void ort206(void) {
}

void ort207(void) {
}

void ort208(void) {
}

void ort209(void) {
}

void ort210(void) {
	// Die Untersuchung der Pilze in der Pilzwaldhöhle ruft diese Funktion auf.
	switch(wuerfel(6)) {
		case 1: textausgabe("Du berührst einen großen Pilzstamm. Unter der Wärme deiner Hand fühlt es sich an, als würde sein Stamm anfangen zu pulsieren.");
				break;
		case 2: textausgabe("Der Pilzstamm fühlt sich warm an, während du mit deiner Hand darüber gehst.");
				break;
		case 3: textausgabe("Du glaubst zu hören, das hinter einer undurchdringlichen Wand aus Pilzen das Geräusch eines fließenden Gewässers erklingt.");
				break;
		case 4: textausgabe("Während du den Pilzhut eines mannshohen Pilzes genauer untersuchst, kannst du dich des Gefühls nicht erwehren, das etwas dich beobachtet.");
				break;
		case 5: textausgabe("Als du den Stamm eines baumhohen Pilzes abklopfst, kommt es dir so vor, als wäre klänge er hohl. Möglicherweise ist der Stamm des Pilzes ja hohl?");
				break;
		default: textausgabe("Du kannst absolut nichts ungewöhnliches wahrnehmen.");
				 break;
	}
	switch(wuerfel(4)) {
		case 1: textausgabe("Vielleicht ist es nur Einbildung, aber du hattest gerade das Gefühl, einen Windstoß im Nacken zu spüren.");
				break;
		case 2: textausgabe("Für einen Moment glaubtest du, ein knackendes Geräusch gehört zu haben.");
				break;
		case 3: textausgabe("Es kommt dir so vor, als wäre es still hier, sehr still, ja, unnatürlich still. Die Stille erscheint dir zu konsequent, als das sie natürlich erscheint.");
				break;
		case 4: textausgabe("Ein ungutes Gefühl breitet sich in deiner Magengrube aus.");
				break;
	}
	// 20% Wahrscheinlichkeit einer Zufallsbegegnung.
	if(wuerfel(10) > 6)
		ort211();
	else
		raumptr[raum](); // weiter geht's im Spiel in Raum [raum]
}

void ort211(void) {
	// Begegnung mit einem Zufallsgegner der oberen Hohlwelt
	int zufallsgegner = wuerfel(7);
	bool kampfausgang;
	charakter_t gegner1 = { "gefiederte Schnecke", 6, 6, 3, 3 };
	charakter_t gegner2 = { "Riesenborkenkäfer", 6, 6, 3, 3 };
	charakter_t gegner3 = { "gigantische Pilzlaus", 3, 3, 4, 4 };
	charakter_t gegner4 = { "wandernder Riesensteinpilz", 5, 5, 4, 4 };
	charakter_t gegner5 = { "Sporenkrabbe", 6, 6, 9, 9 };
	charakter_t gegner6 = { "fleischfressender Pilzaal", 7, 7, 4, 4 };
	charakter_t gegner7 = { "zu groß geratenes Frettchen", 9, 9, 8, 8 };
	switch(zufallsgegner) {
		case 1: textausgabe("Um einen Pilzstamm herum kommt eine Schnecke gekrochen, eine Schnecke, die den Hut eines Pilzes als Haus auf dem Rücken trägt. Plötzlich richtet sie sich auf. Ihr Rücken hat ein Gefieder, das sie ausbreitet, während hier Mund sich weit öffnet, rasiermesserscharfe Zähne zeigt - und ihre Augen sich dir zuwenden.");
				kampfausgang = kampf(&spieler, &gegner1, 1, false, NULL);
				break;
		case 2: textausgabe("Ein Borkenkäfer, in der größe eines Ponys kommt herabgeschwebt und läßt sich vor dir auf dem Boden nieder, augenscheinlich um zu - fressen?");
				if(wuerfel(6) >4)
					textausgabe("Als du dein Gewicht verlagerst, trittst du auf einen kleinen Pilz der jämmerlich zu schreien anfängt. Damit hast du nun die ungeteilte Aufmerksamkeit des Borkenkäfers erlangt.");
				kampfausgang = kampf(&spieler, &gegner2, 1, false, NULL);
				break;
		case 3: textausgabe("Am Stamm eines größeren Pilzes kommt eine Laus heruntergekrabbelt. Ihr Kopf bewegt sich, als würde sie etwas riechen.");
		       		if(wuerfel(6) > 4)
					textausgabe("Und wenn man die Richtung ihrer Bewegung weiter verfolgt, weiß man auch, was sie riecht: Dich!");
				kampfausgang = kampf(&spieler, &gegner3, 1, false, NULL);
				break;
		case 4: if(wuerfel(6) > 4)
					textausgabe("Du hast einen Riesensteinpilz erschreckt. Er dreht sich um und kommt schweren Schrittes auf dich zu.");
				kampfausgang = kampf(&spieler, &gegner4, 1, false, NULL);
				break;
		case 5: if(wuerfel(6) > 4)
					textausgabe("Du hast gerade einer Sporenkrabbe auf die Schere getreten.");
				else
					textausgabe("Du hast gerade einer Sporenkrabbe auf ein Bein getreten.");
				kampfausgang = kampf(&spieler, &gegner5, 1, false, NULL);
				break;
		case 6: if(wuerfel(6) > 3)
					textausgabe("Die Vibration deines Klopfens an einen übergroßen Pilzstamm hat eine Pilzaal hervorgelockt. Die großen, scharfen Zähne in seinem weit aufgerissenen Maul lassen darauf schließen, daß es sich bei ihm um einen Fleischfresser handelt. Und seinem Blick nach zu urteilen handelt es sich bei dir um ein Mittagessen in seinen Augen.");
				else
					textausgabe("Die Vibration deines Klopfens an einen übergroßen Pilzstamm hat einen wütenden Pilzaal hervorgelockt. Bei deinem Anblick reißt er sein Maul weit auf und präsentiert dir eine paar hübsche Reihen rasiermesserscharfe Zähne, die dich sehr an das Maul eines großen weißen Hais erinnern. Du könntest schwören, gerade in deinem Kopf eine Stimme vernommen zu haben, die sagte: \"Hallo Abendessen!\"");
				kampfausgang = kampf(&spieler, &gegner6, 1, false, NULL);
				break;
		default: textausgabe("Wie aus dem Nichts heraus, stürmt plötzlich ein ziemlich großes Frettchen auf dich zu.");
				kampfausgang = kampf(&spieler, &gegner7, 1, false, NULL);
				break;
	}
	if(!kampfausgang)
        beenden(rot, EXIT_SUCCESS, "Das war nicht dein bester Kampf. Um ehrlich zu sein, das war dein schlechtester Kampf - und auch dein letzter Kampf. Dein allerletzter Kampf, den du nicht überlebt hast. Mit dir ist es zu ENDE gegangen.");
	getoetetegegner += 1;
	raumptr[raum](); // weiter geht's im Spiel in Raum [raum]
}

void ort212(void) {
	// Die Suche nach Geheimgängen im ersten Level der Hohlwelt ruft diese Funktion auf.
	switch(wuerfel(6)) {
		case 1: textausgabe("Du hämmerst die Felsen ab, aber von Hohlräumen ist nichts zu hören.");
				break;
		case 2: textausgabe("Die Wand fühlt sich warm an, während du mit deiner Hand darüber gehst.");
				break;
		case 3: textausgabe("Du könntest schwören, hinter der Wand das Geräusch fließenden Wassers hören zu können.");
				break;
		case 4: textausgabe("Während du die Wände abtastest, hast du das Gefühl, beobachtet zu werden.");
				break;
		case 5: textausgabe("Es kommt dir so vor, als würdest sich dein Klopfen etwas hohl anhören. Vielleicht befindet sich ja ein Hohlraum hinter der Wand?");
				break;
		default: textausgabe("Du kannst absolut nichts ungewöhnliches wahrnehmen.");
				 break;
	}
	switch(wuerfel(4)) {
		case 1: textausgabe("Vielleicht ist es nur Einbildung, aber du glaubst gerade ganz deutlich, einen Luftzug gespürt zu haben.");
				break;
		case 2: textausgabe("Für einen Moment glaubtest du, ein Geräusch gehört zu haben.");
				break;
		case 3: textausgabe("Es kommt dir so vor, als wäre es unnatürlich still hier unten.");
				break;
		case 4: textausgabe("Ein ungutes Gefühl breitet sich in deinem Magen aus.");
				break;
	}
	// 20% Wahrscheinlichkeit einer Zufallsbegegnung.
	if(wuerfel(10) > 8)
		ort213();
	else
		raumptr[raum](); // weiter geht's im Spiel in Raum [raum]
}

void ort213(void) {
	// Begegnung mit einem Zufallsgegner der oberen Hohlwelt
	int zufallsgegner = wuerfel(7);
	bool kampfausgang;
	charakter_t gegner1 = { "wandernder Pilz", 2, 2, 3, 3 };
	charakter_t gegner2 = { "Riesenraupe", 6, 6, 3, 3 };
	charakter_t gegner3 = { "Feuerschmetterling", 6, 6, 4, 4 };
	charakter_t gegner4 = { "verschreckte Fledermaus", 5, 5, 4, 4 };
	charakter_t gegner5 = { "Geröllnatter", 6, 6, 5, 5 };
	charakter_t gegner6 = { "Felsegel", 8, 8, 4, 4 };
	charakter_t gegner7 = { "Felsenkrebs", 6, 6, 8, 8 };
	switch(zufallsgegner) {
		case 1: textausgabe("Du hast einen wandernden Riesenpilz angelockt.");
				kampfausgang = kampf(&spieler, &gegner1, 1, false, NULL);
				break;
		case 2: textausgabe("Aus einem Loch in der Wand kommt eine Riesenraupe herangekrochen.");
				if(wuerfel(6) >4)
					textausgabe("Offenbar hat dein ständiges Geklopfe und Geschabe sie aufgeschreckt.");
				kampfausgang = kampf(&spieler, &gegner2, 1, false, NULL);
				break;
		case 3: textausgabe("Ein riesiger roter Schmetterling kommt auf dich zugeschwebt. Das illusorische Farbspiel auf seinen Flügel läßt sie aussehen, als wären sie aus Feuer.");
				kampfausgang = kampf(&spieler, &gegner3, 1, false, NULL);
				break;
		case 4: if(wuerfel(6) > 4)
					textausgabe("Du hast eine Fledermaus aufgeschreckt");
				kampfausgang = kampf(&spieler, &gegner4, 1, false, NULL);
				break;
		case 5: if(wuerfel(6) > 4)
					textausgabe("Du hast gerade einer Felsennatter auf den Kopf gehauen.");
				else
					textausgabe("Die Vibration deines Klopfens hat eine Geröllnatter herbeigelockt.");
				kampfausgang = kampf(&spieler, &gegner5, 1, false, NULL);
				break;
		case 6: if(wuerfel(6) > 3)
					textausgabe("Ein Felsegel schmiegt sich zärtlich um dein Bein.");
				else
					textausgabe("Wie aus dem Nichts heraus, windet sich plötzlich ein Felsegel um deine Hand.");
				kampfausgang = kampf(&spieler, &gegner6, 1, false, NULL);
				break;
		default: textausgabe("Der Fels, auf den du gerade klopfst, war ein Felsenkrebs.");
				kampfausgang = kampf(&spieler, &gegner7, 1, false, NULL);
				break;
	}
	if(!kampfausgang)
        beenden(rot, EXIT_SUCCESS, "Das war nicht dein bester Kampf. Um ehrlich zu sein, das war dein schlechtester Kampf - und auch dein letzter Kampf. Dein allerletzter Kampf, den du nicht überlebt hast. Mit dir ist es zu ENDE gegangen.");
	getoetetegegner += 1;
	raumptr[raum](); // weiter geht's im Spiel in Raum [raum]
}

void ort214(void) {
}

void ort215(void) {
}

void ort216(void) {
}

void ort217(void) {
}

void ort218(void) {
}

void ort219(void) {
}

void ort220(void) {
}

void ort221(void) {
}

void ort222(void) {
}

void ort223(void) {
}

void ort224(void) {
}

void ort225(void) {
}

void ort226(void) {
}

void ort227(void) {
}

void ort228(void) {
}

void ort229(void) {
}

void ort230(void) {
}

void ort231(void) {
}

void ort232(void) {
}

void ort233(void) {
}

void ort234(void) {
}

void ort235(void) {
}

void ort236(void) {
}

void ort237(void) {
}

void ort238(void) {
}

void ort239(void) {
}

void ort240(void) {
}

void ort241(void) {
}

void ort242(void) {
}

void ort243(void) {
}

void ort244(void) {
}

void ort245(void) {
}

void ort246(void) {
}

void ort247(void) {
}

void ort248(void) {
}

void ort249(void) {
}

void ort250(void) {
}

void ort251(void) {
}

void ort252(void) {
}

void ort253(void) {
}

void ort254(void) {
}

void ort255(void) {
}

void ort256(void) {
}

void ort257(void) {
}

void ort258(void) {
}

void ort259(void) {
}

void ort260(void) {
}

void ort261(void) {
}

void ort262(void) {
}

void ort263(void) {
}

void ort264(void) {
}

void ort265(void) {
}

void ort266(void) {
}

void ort267(void) {
}

void ort268(void) {
}

void ort269(void) {
}

void ort270(void) {
}

void ort271(void) {
}

void ort272(void) {
}

void ort273(void) {
}

void ort274(void) {
}

void ort275(void) {
}

void ort276(void) {
}

void ort277(void) {
}

void ort278(void) {
}

void ort279(void) {
}

void ort280(void) {
}

void ort281(void) {
}

void ort282(void) {
}

void ort283(void) {
}

void ort284(void) {
}

void ort285(void) {
}

void ort286(void) {
}

void ort287(void) {
}

void ort288(void) {
}

void ort289(void) {
}

void ort290(void) {
}

void ort291(void) {
}

void ort292(void) {
}

void ort293(void) {
}

void ort294(void) {
}

void ort295(void) {
}

void ort296(void) {
}

void ort297(void) {
}

void ort298(void) {
}

void ort299(void) {
}

// -----------
// --- 300 ---
// -----------

void ort300(void) {
}

void ort301(void) {
}

void ort302(void) {
}

void ort303(void) {
}

void ort304(void) {
}

void ort305(void) {
}

void ort306(void) {
}

void ort307(void) {
}

void ort308(void) {
}

void ort309(void) {
}

void ort310(void) {
}

void ort311(void) {
}

void ort312(void) {
}

void ort313(void) {
}

void ort314(void) {
}

void ort315(void) {
}

void ort316(void) {
}

void ort317(void) {
}

void ort318(void) {
}

void ort319(void) {
}

void ort320(void) {
}

void ort321(void) {
}

void ort322(void) {
}

void ort323(void) {
}

void ort324(void) {
}

void ort325(void) {
}

void ort326(void) {
}

void ort327(void) {
}

void ort328(void) {
}

void ort329(void) {
}

void ort330(void) {
}

void ort331(void) {
}

void ort332(void) {
}

void ort333(void) {
}

void ort334(void) {
}

void ort335(void) {
}

void ort336(void) {
}

void ort337(void) {
}

void ort338(void) {
}

void ort339(void) {
}

void ort340(void) {
}

void ort341(void) {
}

void ort342(void) {
}

void ort343(void) {
}

void ort344(void) {
}

void ort345(void) {
}

void ort346(void) {
}

void ort347(void) {
}

void ort348(void) {
}

void ort349(void) {
}

void ort350(void) {
}

void ort351(void) {
}

void ort352(void) {
}

void ort353(void) {
}

void ort354(void) {
}

void ort355(void) {
}

void ort356(void) {
}

void ort357(void) {
}

void ort358(void) {
}

void ort359(void) {
}

void ort360(void) {
}

void ort361(void) {
}

void ort362(void) {
}

void ort363(void) {
}

void ort364(void) {
}

void ort365(void) {
}

void ort366(void) {
}

void ort367(void) {
}

void ort368(void) {
}

void ort369(void) {
}

void ort370(void) {
}

void ort371(void) {
}

void ort372(void) {
}

void ort373(void) {
}

void ort374(void) {
}

void ort375(void) {
}

void ort376(void) {
}

void ort377(void) {
}

void ort378(void) {
}

void ort379(void) {
}

void ort380(void) {
}

void ort381(void) {
}

void ort382(void) {
}

void ort383(void) {
}

void ort384(void) {
}

void ort385(void) {
}

void ort386(void) {
}

void ort387(void) {
}

void ort388(void) {
}

void ort389(void) {
}

void ort390(void) {
}

void ort391(void) {
}

void ort392(void) {
}

void ort393(void) {
}

void ort394(void) {
}

void ort395(void) {
}

void ort396(void) {
}

void ort397(void) {
}

void ort398(void) {
}

void ort399(void) {
}

void ort400(void) {
}


// Implementation Teste dein Glück
bool tdg(charakter_t *figur) {
	if(figur->glueck <= 0)

		return false; // Da kann man nur noch Pech haben!
	if((wuerfel(6) + wuerfel(6)) <= figur->glueck--)
		return true; // Glück gehabt
	return false; // Pech gehabt
}

// Implementation: Kampfrunde
bool kampfrunde(charakter_t *angreifer, charakter_t *verteidiger, void (*fluchtpunkt)()) {
	int testgewandheit[2];
	bool glueckstest = false;
	bool wirklichglueck;
	int schildbonus = 0;

	testgewandheit[0] = angreifer->gewandheit + wuerfel(6) + wuerfel(6) + angriffsbonus + paralysiert;
	testgewandheit[1] = verteidiger->gewandheit + wuerfel(6) + wuerfel(6);
	if(unsichtbar) // Falls du unsichtbar bist
		testgewandheit[0] += 2;
	if(testgewandheit[0] == testgewandheit[1])
		return true; // Unentschieden, das heißt - nicht getroffen
	momentane_werte(angreifer);
	momentane_werte(verteidiger);
	// Möchte der Spieler fliehen?
	if(fluchtpunkt != NULL)
		if(janeinfrage("Möchtest du fliehen (j/n)? "))
			flucht(fluchtpunkt);
	// Möchte der Spieler sein Glück versuchen?
	if(angreifer->glueck > 0) {
		if(janeinfrage("Möchtest du dein Glück testen (j/n)? ")) {
			glueckstest = true;
			wirklichglueck = tdg(angreifer);
		}
	}
	// Die Auswertung des Kampfes
	if(testgewandheit[0] > testgewandheit[1]) { // Der Spieler geht als Sieger hervor
		if((nursilbertrifft == true) && (silberwaffe == false)) // Nur falls das Monster imun ist gegen Silber
			return true; // Kein Treffer abbekommen
		if(unsichtbar) // Unsichtbare treffen besser
			verteidiger->staerke -= 2;
		if(((objekt[gewehr] > 0) || (objekt[pistole] > 0)) && (objekt[patrone] > 0)) { // Schusswaffe = + 1 Punkt, solange Patrone vorhanden
			verteidiger->staerke -= 1;
			objekt[patrone] -= 1;
		}
		if(glueckstest) {
			if(wirklichglueck)
				verteidiger->staerke -= 4;
			else
				verteidiger->staerke -= 1;
			return true; // Kein Treffer abbekommen
		}
		verteidiger->staerke -= 2;
		return true; // Kein Treffer abbekommen
	}
	else if (testgewandheit[0] < testgewandheit[1]) { // Der Spieler verliert die Runde
		if((objekt[schild] >= 0) && (wuerfel(6) == 6)) // Der Schild fängt den Schlag ab
			schildbonus = 1;
		if(unsichtbar) // Der Unsichtbarkeitsbonus
			switch(wuerfel(6)) {
				case 2:
				case 4:
					angreifer->staerke += 1;
					break;
				case 6:
					return true; // Kein Treffer erhalten, dank Unsichtbarkeit
				default:
					break;
			}
		if(glueckstest) {
			if(wirklichglueck)
				angreifer->staerke -= 1 - schildbonus;
			else
				angreifer->staerke -= 4 - schildbonus;
			return false; // Treffer abbekommen
		}
		angreifer->staerke -= 2 - schildbonus;
		return false; // Treffer abbekommen
	}
	return true; // Unentschieden, das heißt - nicht getroffen
}

// Implementation: Kampf
bool kampf(charakter_t *spieler, charakter_t *gegner, int anzahl, bool trefferverboten, void (*fluchtpunkt)()) {
	int gesamtstaerke = 0;
	bool keintreffererhalten = true;

	for(int i=0; i < anzahl; i++)
		gesamtstaerke += gegner[i].staerke;
	while((spieler->staerke > 0) && (gesamtstaerke > 0)) {
		for(int i=0; i < anzahl; i++)
			if(spieler->staerke > 0) {
				if(gegner[i].staerke > 0)
					keintreffererhalten = kampfrunde(spieler, &gegner[i], fluchtpunkt);
				if(trefferverboten)
					if(!keintreffererhalten)
						return false;
			}
			else
				return false;
		// Leben die Gegner noch?
		gesamtstaerke = 0;
		for(int i=0; i < anzahl; i++) {
			if(gegner[i].staerke < 0)
				gegner[i].staerke = 0;
			gesamtstaerke += gegner[i].staerke;
		}
	}
	if(spieler->staerke > 0)
		return true; // Sieger im Kampf ^.^
	return false; // Der Spieler ist tot v.v
}

// Implementation: Momentane Werte
void momentane_werte(charakter_t *person) {
	textausgabe("\n      Name: %s\n", person->name);
	textausgabe("Gewandheit: %2d / %2d\n", person->gewandheit, person->gewandheit_start);
	textausgabe("    Stärke: %2d / %2d\n", person->staerke, person->staerke_start);
	textausgabe("     Glück: %2d / %2d\n", person->glueck, person->glueck_start);
}





// Implementation: Auswahl
void auswahl(char *beschreibung, int maxzahl, ...) {
	char eingabe[20];
	int ergebnis = 0;
	char zusatzbeschreibung[300];
	va_list zeiger;
	// reserviere Platz für alle Auswahlmöglichkeiten
	void (*fptr[maxzahl]) (void);
	// Werte der VA-Liste in die Funktionszeiger kopieren
	va_start(zeiger, maxzahl); // Die maxzahl Zeiger auslesen
	for(int i=0; i < maxzahl; ++i)
		fptr[i] = va_arg(zeiger, void*);
	va_end(zeiger);

	// Die Zusatzfunktionen
	while((ergebnis < 1) || (ergebnis > maxzahl)) {
		vordergrundfarbe(zyan);
		strcpy(zusatzbeschreibung, " ");
		if(objekt[gewandheitstrank] > 0)
			strcat(zusatzbeschreibung, "(44) Gewandheitstrank trinken ");
		if(objekt[staerketrank] > 0)
			strcat(zusatzbeschreibung, "(55) Stärketrank trinken ");
		if(objekt[glueckstrank] > 0)
			strcat(zusatzbeschreibung, "(66) Glückstrank trinken ");
		strcat(zusatzbeschreibung, "(77) Spielstand laden (88) Spielstand speichern (99) Speichern & beenden");
		textausgabe(beschreibung);
        textausgabe(zusatzbeschreibung);
        vordergrundfarbe(gelb);
		textausgabe("Du wählst: ");
        vordergrundfarbe(gruen);
        texteingabe(eingabe, 20);
		ergebnis = atoi(eingabe);
		vordergrundfarbe(weiss);
		switch(ergebnis) {
			case 44: gewandheitstrank_trinken();
					break;
			case 55: staerketrank_trinken();
					break;
			case 66: glueckstrank_trinken();
					break;
			case 77: laden();
					 break;
			case 88: speichern();
					 break;
			case 99: speichern();
                     beenden(gruen, EXIT_SUCCESS, "Bis bald!");
			default: break;
		}
	}
	fptr[ergebnis-1](); // Umwandlung menschliche Nummerierung in Arraynummerierung
}

// Funktion: VersucheDeinGlueck
void versuchedeinglueck(void (*funktion1)(), void (*funktion2)()) {
	vordergrundfarbe(gruen);
	textausgabe("--- Versuche dein Glück! - Drücke ENTER ---");
//	getchar();
	getch();
	vordergrundfarbe(weiss);
	if(tdg(&spieler)) // Glück gehabt
		funktion1();
	else 			// Pech gehabt
		funktion2();
}

// Funktion: VersucheDeineGewandheit
void versuchedeinegewandheit(void (*funktion1)(), void (*funktion2)()) {
	bool gewandheit;

	vordergrundfarbe(gruen);
	textausgabe("--- Versuche deine Gewandheit! - Drücke ENTER ---");
	getch();
	vordergrundfarbe(gelb);
	gewandheit = wuerfel(6) + wuerfel(6);
	textausgabe("Deine momentane Gewandheit ist %d, dein Würfelergebnis ist %d\n", spieler.gewandheit, gewandheit);
	vordergrundfarbe(weiss);
    if(gewandheit <= spieler.gewandheit) // Glück gehabt
		funktion1();
	else 			// Pech gehabt
		funktion2();
}


// Funktion: Flucht
void flucht(void (*funktion1)()) {
    vordergrundfarbe(rot);
	textausgabe("Du entschließt dich zu fliehen, was dich 2 Stärkepunkte kosten wird.");
    vordergrundfarbe(weiss);
	if(spieler.glueck > 0) { // Ist ein Glückstest möglich?
		textausgabe("Du könntest natürlich auch dein Glück auf die Probe stellen. Hast du Glück, ist es eine geordnete Flucht - und du verlierst nur 1 Stärkepunkt. Wenn du jedoch kein Glück hast, wird es zu einer heilosen Flucht und du verlierst 3 Stärkepunkte.");
		if(janeinfrage("Möchtest du dein Glück testen (j/n)? ")) {
			if(tdg(&spieler)) // Glück gehabt
				spieler.staerke -= 1;
			else // Pech gehabt
				spieler.staerke -= 3;
		}
		else
			spieler.staerke -= 2;
	}
	else {
		spieler.staerke -= 2;
	}
	if(spieler.staerke) {
		funktion1();
        beenden(rot, EXIT_FAILURE, "Fehler!\nEine Ortsvariable ist wohl noch leer.\nDer letzer bekannte Raum war %d.", raum);
	}
	else 
        beenden(rot, EXIT_SUCCESS, "Du bist zu geschwächt, um auch nur einen einzigen weiteren Atemzug zu machen. Unter Schmerzen entweicht dir Atemluft aus deinen Lungen. Die Umgebung wird erst Rot vor deinen Augen und gleitet schließlich ins Schwarze ab. Das ist dein ENDE.");
	beenden(rot, EXIT_FAILURE, "Fehler!\nIch bin am Ende der Fluchtroutine angelangt. Der letzte bekannte Raum war %d.", raum);
}

// Implementation: Mahlzeit
void mahlzeit(void) {
	if(janeinfrage("Möchtest du eine Mahlzeit zu dir nehmen (j/n)? "))
		if(objekt[proviant] > 0) {
			textausgabe("Nachdem du dich versichert hast, das niemand in der Nähe ist, entnimmst du ein Proviantpaket aus deinem Rucksack. Genüsslich und so leise wie möglich verzehrst du es. Du kannst spüren, wie etwas Kraft in deinen müden Körper zurückkehrt.");
			objekt[proviant] -= 1;
			staerkesteigerung(4, 0);
		}
		else
			textausgabe("Nachdem du dich versichert hast, das niemand in der Nähe ist, durchwühlst du deinen Rucksack auf der Suche nach einem Proviantpaket. Nach einigen Minuten und mehrfachen aus- und einpacken des Rucksacks gibst du verzweifelt auf. Es ist tatsächlich kein einziger Brotkrummen mehr übrig.");
}

// Funktion: Gewandheitstrank trinken
void gewandheitstrank_trinken() {
	if(objekt[gewandheitstrank] > 0) {
		objekt[gewandheitstrank] -= 1;
		spieler.gewandheit = spieler.gewandheit_start;
		textausgabe("Du trinkst den Gewandheitstrank. Deine Gewandheit könnte nicht besser sein.");
	}
}

// Funktion: Stärketrank trinken
void staerketrank_trinken() {
	if(objekt[staerketrank]) {
		objekt[staerketrank] -= 1;
		spieler.staerke = spieler.staerke_start;
		textausgabe("Du trinkst den Stärketrank. Deine Stärke könnte nicht besser sein.");
	}
}

// Funktion Glückstrank trinken
void glueckstrank_trinken() {
	if(objekt[glueckstrank]) {
		objekt[glueckstrank] -= 1;
		spieler.glueck_start += 1;
		spieler.glueck = spieler.glueck_start;
		textausgabe("Du trinkst den Glückstrank. Deine Glück ist besser als jemals zuvor.");
	}
}

// Implementation: Objkekt ablegen
void objekt_ablegen(void) {
	char eingabe[21];
	int ergebnis = -1;
	char bestaetigung;
	int j=0;

	for(int i=0; (i < maximalobjekt) && (objekt[i] > 0); i++) {
		textausgabe("(%d) %s ", objekt[i], objektname[i]);
		j++;
		if(!(j % 3))
			textausgabe("\n");
	}
	while((ergebnis < 0) || (ergebnis >= maximalobjekt)) {
	    vordergrundfarbe(gruen);
        textausgabe("Bitte gib die Nummer des abzulegenden Objektes an! ");
        vordergrundfarbe(rot);
        texteingabe(eingabe, 20);
		ergebnis = atoi(eingabe);
	}
	vordergrundfarbe(gelb);
	textausgabe("%s wirklich ablegen? ", objektname[ergebnis]);
	bestaetigung = taste();
	vordergrundfarbe(weiss);
	if((bestaetigung == 'j') || (bestaetigung == 'J')) {
	    objekt[ergebnis] -= 1;
        vordergrundfarbe(gelb);
		textausgabe("\n%s abgelegt.\n", objektname[ergebnis]);
        vordergrundfarbe(weiss);
	}
}


// Implementation: gewandheitssteigerung
void gewandheitssteigerung(int temporaer, int permanent) {
	spieler.gewandheit_start += permanent;
	spieler.gewandheit += temporaer;
	if(spieler.gewandheit > spieler.gewandheit_start)
		spieler.gewandheit = spieler.gewandheit_start;
}

// Implementation: staerkesteigerung
void staerkesteigerung(int temporaer, int permanent) {
	spieler.staerke_start += permanent;
	spieler.staerke+= temporaer;
	if(spieler.staerke > spieler.staerke_start)
		spieler.staerke = spieler.staerke_start;
}

// Implementation: glueckssteigerung
void glueckssteigerung(int temporaer, int permanent) {
	spieler.glueck_start += permanent;
	spieler.glueck += temporaer;
	if(spieler.glueck > spieler.glueck_start)
		spieler.glueck = spieler.glueck_start;
}

// -------------------------
// Implementation: Speichern
// -------------------------

int speichern(void) {
	FILE *datei;
	datei = fopen(DATEINAME, "w");
	if(ferror(datei)) {
        hinweis(rot, "Fehler!\nDie Datei läßt sich nicht speichern. Fahre ohne gespeicherten Spielstand fort.");
		return 1;
	}

	fprintf(datei, "%s\n", spieler.name);
	fprintf(datei, "%d\n", spieler.gewandheit);
	fprintf(datei, "%d\n", spieler.gewandheit_start);
	fprintf(datei, "%d\n", spieler.glueck);
	fprintf(datei, "%d\n", spieler.glueck_start);
	fprintf(datei, "%d\n", spieler.staerke);
	fprintf(datei, "%d\n", spieler.staerke_start);
	fprintf(datei, "%d\n", raum);
	for(int i=0; i<maximalobjekt; i++)
		fprintf(datei, "%d\n", objekt[i]);
	fprintf(datei, "%d\n", angriffsbonus);
	fprintf(datei, "%d\n", fuenfwahl);
	fprintf(datei, "%d\n", (int) nursilbertrifft);
	fprintf(datei, "%d\n", paralysiert);
	fprintf(datei, "%d\n", preis);
	fprintf(datei, "%d\n", schluessel);
	fprintf(datei, "%d\n", (int) schluessel9);
	fprintf(datei, "%d\n", (int) schluessel66);
	fprintf(datei, "%d\n", (int) schluessel99);
	fprintf(datei, "%d\n", (int) schluessel111_1);
	fprintf(datei, "%d\n", (int) schluessel111_2);
	fprintf(datei, "%d\n", (int) schluessel125);
	fprintf(datei, "%d\n", (int) schluesselbootshaus);
	fprintf(datei, "%d\n", (int) schummeln);
	fprintf(datei, "%d\n", (int) silberwaffe);
	fprintf(datei, "%d\n", (int) unsichtbar);
	fprintf(datei, "%d\n", (int) tripodgesehen);
	fprintf(datei, "%d\n", getoetetemenschen);
	fprintf(datei, "%d\n", getoetetegegner);
	fprintf(datei, "%d\n", (int) agartha);
	fprintf(datei, "%d\n", (int) verzeichnisgelesen);
	fprintf(datei, "%d\n", (int) buchgefunden);
	fprintf(datei, "%d\n", (int) kartegefunden);
	fprintf(datei, "%d\n", (int) sargverschoben);
	fprintf(datei, "%d\n", (int) durchganggeoeffnet);
	fprintf(datei, "%d\n", (int) schluesselgefunden);
	fprintf(datei, "%d\n", (int) dreistelzer);
	fprintf(datei, "%d\n", rotation);
	fprintf(datei, "%d\n", (int) dracheverletzt);
	fprintf(datei, "%d\n", (int) drachetot);
	fprintf(datei, "%d\n", minenzwerge);
	fprintf(datei, "%d\n", stollentroll);
	fprintf(datei, "%d\n", (int) gitteroffen);
	fprintf(datei, "%d\n", (int) raetsel1);
	fprintf(datei, "%d\n", (int) raetsel2);
	fprintf(datei, "%d\n", (int) raetsel3);
	fprintf(datei, "%d\n", (int) raetsel4);
	fprintf(datei, "%d\n", (int) raetsel5);
	fprintf(datei, "%d\n", (int) dwellmer);
	fprintf(datei, "%d\n", arianna);
	fprintf(datei, "%d\n", elke);
	fprintf(datei, "%d\n", (int) schluesselarianna);
	fprintf(datei, "%d\n", verloben);
    hinweis(gruen, "Spielstand gespeichert.\nRaum: %d\n", raum);
    fflush(datei);
    fclose(datei);
	return 0;
}


// -------------------------
// Implementation: Laden
// -------------------------

int laden(void) {
	char eingabe[100];
	FILE *datei;
	datei = fopen(DATEINAME, "r");
	if(ferror(datei)) {
		vordergrundfarbe(rot);
		textausgabe("Fehler!\nDie Datei ließ sich nicht öffnen. Fahre ohne geladenen Spielstand fort.");
		vordergrundfarbe(weiss);
		return 1;
	}
	fgets(eingabe, 100, datei);
	for(int i=0; i < strlen(eingabe); i++)
		if(eingabe[i] == '\n')
			eingabe[i] = '\0';
	strcpy(spieler.name, eingabe);
	fgets(eingabe, 100, datei);
	spieler.gewandheit = atoi(eingabe);
	fgets(eingabe, 100, datei);
	spieler.gewandheit_start = atoi(eingabe);
	fgets(eingabe, 100, datei);
	spieler.glueck = atoi(eingabe);
	fgets(eingabe, 100, datei);
	spieler.glueck_start = atoi(eingabe);
	fgets(eingabe, 100, datei);
	spieler.staerke = atoi(eingabe);
	fgets(eingabe, 100, datei);
	spieler.staerke_start = atoi(eingabe);
	fgets(eingabe, 100, datei);
	raum = atoi(eingabe);
	for(int i=0; i<maximalobjekt; i++) {
		fgets(eingabe, 100, datei);
		objekt[i] = atoi(eingabe);
	}
	fgets(eingabe, 100, datei);
	angriffsbonus = atoi(eingabe);
	fgets(eingabe, 100, datei);
	fuenfwahl = atoi(eingabe);
	fgets(eingabe, 100, datei);
	nursilbertrifft = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	paralysiert = atoi(eingabe);
	fgets(eingabe, 100, datei);
	preis = atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluessel = atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluessel9 = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluessel66 = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluessel99 = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluessel111_1 = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluessel111_2 = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluessel125 = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluesselbootshaus = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	schummeln = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	silberwaffe = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	unsichtbar = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	tripodgesehen = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	getoetetemenschen = atoi(eingabe);
	fgets(eingabe, 100, datei);
	getoetetegegner = atoi(eingabe);
	fgets(eingabe, 100, datei);
	agartha = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	verzeichnisgelesen = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	buchgefunden = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	kartegefunden = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	sargverschoben = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	durchganggeoeffnet = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluesselgefunden = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	dreistelzer = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	rotation = (unsigned int) atoi(eingabe);
	fgets(eingabe, 100, datei);
	dracheverletzt = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	drachetot = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	minenzwerge = (int) atoi(eingabe);
	fgets(eingabe, 100, datei);
	stollentroll = (int) atoi(eingabe);
	fgets(eingabe, 100, datei);
	gitteroffen = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	raetsel1 = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	raetsel2 = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	raetsel3 = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	raetsel4 = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	raetsel5 = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	dwellmer = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	arianna = (int) atoi(eingabe);
	fgets(eingabe, 100, datei);
	elke = (int) atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluesselarianna = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	verloben = (int) atoi(eingabe);
	fclose(datei);

    hinweis(gruen, "Spielstand geladen. Raum: %d\n", raum);
	momentane_werte(&spieler);
	raumptr[raum](); // weiter geht's im Spiel in Raum [raum]
	return 0;
}

// Funktion: quit
void quit() {
	endwin();
}

// Funktion: Rätsel
bool raetsel(char *raetseltext, char *antworttext) {
	char eingabe[41];
    vordergrundfarbe(magenta);
	textausgabe(raetseltext);
    vordergrundfarbe(gelb);
    textausgabe("Bitte beantworte das Rätsel mit der Eingabe eines einzigen Wortes!");
    vordergrundfarbe(zyan);
    texteingabe(eingabe, 40);
    vordergrundfarbe(weiss);
	if(0 == strcmp(antworttext, eingabe))
		return true;
	else
		return false;
}

// Funktion: Zweisamkeit
void zweisamkeit(int wert) {
	arianna += wert;
	switch(arianna) {
		case 20: textausgabe("Euch beiden gefällt es, zusammen etwas zu unternehmen, tatsächlich könnte man sagen, das ihr echte Freunde geworden seid.");
			 break;
		case 40: textausgabe("In der letzten Zeit wurde es euch immer bewußter, daß die gemeinsame Zeit für euch beide die beste Zeit des Tages ist. Nach einem ausführlichen Gespräch seid ihr zu dem Ergebnis gekommen, das es nicht verkehrt werde, den Großteil der Zeit ab sofort gemeinsam zu verbringen.");
			 break;
		case 60: textausgabe("Arianna sieht dich verstohlen an. Sie gibt dir verschmitzt einen Kuss auf den Mund und läßt einen Schlüssel in deine Hand gleiten. \"Mein Heim soll ab jetzt auch dein Heim sein!\" sagt sie dabei und errötet kräftig unter dem Flaum ihres Backenbarts.");
			 schluesselarianna = true;
			 break;
		case 80: textausgabe("Arianna druckst etwas herum, schließlich aber bringt sie auf den Punkt, was sie sagen möchte: \"Was hältst du davon, wenn wir den Ewigen Bund der Schmiede eingehen?\" Sie sieht jetzt irgendwie schüchtern aus.");
			 if(janeinfrage("Willst du dich mit Arianna verloben (j/n)?")) {
				textausgabe("Du mußt nicht einen Augenblick lang nachdenken, sondern grinst frech zurück: \"Na klar!\" und erhältst von ihr einen Knuff der dich zu Boden schickt, dann liegt sie auch schon auf dir und bedeckt dein Gesicht mit Küssen. Und dann wird sie plötzlich wieder ernst: \"Dann wirst du dich mit meinen Eltern treffen müssen - sie müssen dem Bund zustimmen!\"\nIrgendwie war es klar, das nichts im Leben besonders einfach isti");
				verloben = 1;
			 } else {
				 textausgabe("Ganz liebevoll sagst du ihr, daß du das zum jetzigen Zeitpunkt nicht für eine gute Idee hältst - und erhältst dafür ein Schmollgesicht von ihr, wie du es noch nie zuvor gesehen hast.");
				 arianna = 61;
			 }
			 break;
		case 99: if(verloben < 6)
				 arianna = 98;
			 else
				 textausgabe("Jetzt ist alles perfekt! Ihr beide solltet so bald als möglich in die große Schmiede gehen!");
			 break;
		case 100: if((verloben >= 6) && (raum == 96))
				textausgabe("Jetzt ist der große Tag da, an dem du mit Arianna den Bund eingehen wirst!");
			  else
				  arianna = 99;
			  break;
		default: break;
	}
}


// ----------------
// Die Hauptroutine
// ----------------
int main(void) {
	bool spielerlebt = true;

    zufall_per_zeit();
    ncurses_init(quit);
	vordergrundfarbe(blau);
	textausgabe("--------------------------");
	textausgabe("Steampunk FFR - Der Anfang");
	textausgabe("--------------------------");
	hinweis(gelb, "Ein \"Das-ist-dein-Abenteuer\" Roman\n");
	hinweis(magenta, "Nach einer Geschichte von Sascha Biermanns\n");
    vordergrundfarbe(weiss);
	if(janeinfrage("Möchtest du ein gespeichertes Spiel fortführen (j/n)?"))
		laden();
	intro();
	vorwort();
	ort1();
}
