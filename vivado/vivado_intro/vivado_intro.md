Vorbemerkungen
==============

Diese Anleitung dient als Selbstlernkurs zur Einführung in die Xilinx
Vivado Entwicklungsumgebung. Die Anleitung beschreibt anhand eines
einfachen VHDL-Beispiels die notwendigen Schritte in Vivado, um von der
Eingabe der VHDL-Quellcodes bis zur Erzeugung eines Bit-Files für die
Programmierung eines FPGAs zu kommen. Die Anleitung bezieht sich auf die
Vivado Version 2021.1 (oder identisch 2020.1 - 2020.3), mit der die
einzelnen Schritte getestet wurden. Die Vivado-Software kann von der
Xilinx-Homepage als kostenfreie Version heruntergeladen und installiert
werden: "http://www.xilinx.com/support/download.html". Wählen Sie
"Vivado" und "2021.1" (oder ältere Version) aus und dann " Xilinx
Unified Installer 2021.1: Windows Self Extracting Web Installer" (oder
ggf. den Linux Web Installer). Sie müssen am Ende der Installation eine
Lizenz anfordern, hierzu müssen Sie sich bei Xilinx registrieren.

Wenn Sie das entstandene Design auch auf einer FPGA-Hardware testen
möchten, dann wäre beispielsweise das "Basys 3"-FPGA-Board von Digilent
empfehlenswert, welches man in Deutschland z.B. von
"trenz-electronic.de" bekommen kann. Diese Anleitung behandelt jedoch
nicht die Spezifikation des Boards und eventuell notwendige
Debug-Arbeiten. Wir verwenden allerdings den entsprechenden
Artix-7-FPGA-Baustein für das hier beschriebene Vivado-Projekt als
Ziel-Hardware. Sie benötigen das Board nicht, um diese Anleitung mit
Hilfe der Vivado-Software durchzuarbeiten. Für weitere und tiefergehende
Informationen seien die Xilinx Dokumentationen empfohlen.

Erstellen eines Projekts
========================

In diesem Abschnitt werden wir ein Vivado-Projekt für den entsprechenden
Ziel-FPGA-Baustein erstellen.


-   Erstellen Sie auf Ihrem Entwicklungsrechner ein Stammverzeichnis für
    die Vivado-Projekte, z.B. in Ihrem Benutzer-Verzeichnis oder an
    einer anderen Stelle, wo Sie als Benutzer Schreib- und Leserechte
    haben

-   Starten Sie Vivado 2021.1 Über das Windows-Startmenü (üblicherweise
    unter "Xilinx Design Tools" zu finden).

-   Legen Sie unter "Quick Start" mit "Create Project" ein neues Projekt
    an.
    Es öffnet sich ein "Wizard": Tragen Sie in die Masken die
    entsprechenden Einträge ein, wie sie nachfolgend beschrieben sind,
    und gehen Sie jeweils mit "Next" zur nächsten Maske.

-   Geben Sie
    Ihrem Projekt einen nachvollziehbaren Namen (z.B. intro1). Wählen
    Sie als "Project location" das zuvor erstellte Stammverzeichnis für
    die Vivado-Projektdateien und aktivieren Sie die Option "Create
    project subdirectory". Achtung: Verwenden Sie in Pfad- oder
    Dateinamen keine Leer- und Sonderzeichen oder Umlaute!



-   Wählen Sie
    "RTL Project" als Projekttyp und wählen Sie "Do not specify sources
    at this time". Wir werden die VHDL-Quellen erst sp�ter anlegen.


-   Die Auswahl
    des FPGA-Bausteins, der auf dem BASYS3-Board verbaut ist, erfolgt am
    schnellsten durch das Setzen der folgenden Einstellungen: "Family:
    Artix-7" "Package: cpg236" "Speed grade: -1" Wählen Sie aus der
    Liste den Baustein "xc7a35tcpg236-1" und schliessen Sie die
    Projekterstellung mit "Next", bzw. "Finish" im folgenden Dialog ab.
    Die korrekte Angabe des FPGA-Bausteins ist insbesondere wichtig für
    die Implementierung und die korrekte Zuweisung der Pins. Wird der
    falsche Baustein ausgewählt, so wird man das FPGA später nicht
    korrekt konfigurieren können. Die Erstellung des Vivado-Projektes
    ist damit abgeschlossen.

Hinzufügen der VHDL-Quelldateien
================================

In diesem Abschnitt werden wir die VHDL-Quelldatei für unser Design
erstellen und eine entsprechende VHDL-Testbench für die Simulation. Bei
dem Design soll es sich um einen 4-Bit Zähler handeln, der mit jeder
steigenden Flanke eines Eingangs inkrementiert. Dieser Eingang kann auf
einen Taster des Basys-Boards gelegt werden. Die vier Zählerausgänge
können auf die LEDs des Boards geschaltet werden. Den Quellcode für den
Zähler finden Sie im Anhang.

-   Klicken Sie im "Flow Navigator" (linke Spalte im
    Vivado-Fenster) unter "Project Manager" auf "Add Sources".


-   Im aufstartenden "Wizard" wählen Sie "Add or Create Design Sources".
    Mit "Next" gehen Sie in die nächste Maske und wählen "Create File"
    . Wählen
    Sie "VHDL" als "File type" aus und tragen Sie "counter.vhd" als Name
    ein. Dr�cken Sie "OK" und dann "Finish" zum Abschluss.



-   In der folgenden Maske können Sie die Ports Ihres Designs definieren.
    Geben Sie die Ports wie in der Abbildung gezeigt ein: "count" ist
    der Eingang, mit dessen steigender Flanke der Zähler inkrementieren
    soll und "ctr\_out" ist der Ausgang des Zählers, dieser wird als
    4-Bit Bus angelegt. Nach Abschluss wird ein VHDL-Code generiert,
    dieser sollte nun im "Project Manager"-Fenster von Vivado unter
    "Sources"$\rightarrow$\"'Design Sources\"' erscheinen. Öffnen Sie
    die Datei im Editor durch Doppelklick. Sie haben nun ein
    VHDL-\"'Ger�st\"' mit fertiger Entity und einer Architecture
    vorliegen. Erg�nzen Sie nun die noch fehlenden Zeilen mit dem Code
    aus dem Anhang. Wenn Sie die Datei abspeichern ("Save file"-Knopf
    oben im Editor oder "Strg-S") wird eine grobe Syntaxpr�fung gemacht.
    Falls Fehler im Code sind, so werden diese im Editor farbig
    angezeigt. Korrigieren Sie die Fehler.


-   Nun fehlt noch die Testbench: Klicken Sie wieder auf "Add Sources"
    im "Flow Navigator". Wählen Sie nun aber "Add or Create
    **Simulation** Sources". Dies ist wichtig, damit Vivado wei�, dass
    die Testbench nur für die Simulation da ist und nicht in Hardware
    umgesetzt werden soll. Tragen Sie analog zur vorherigen
    Vorgehensweise wieder "VHDL" als Typ ein und den Namen
    "counter\_tb.vhd". In der Port-Maske tragen Sie keine Ports ein - es
    ist ja eine Testbench. Beantworten Sie die nachfolgende Frage beim
    Abschluss der Maske mit "Yes". Wenn Sie nun in Vivado im "Project
    Manager" die "Simulation Sources" aufklappen, dann sehen Sie das
    Design und die Testbench aufgelistet.

-   Editieren Sie nun die Testbench und fügen Sie den Code aus dem
    Anhang hinzu. Wenn Sie alles richtig eingegeben haben, dann sollte
    auch die Hierarchie wie in Abbildung angezeigt werden. Wenn dies nicht der Fall ist, dann
    stimmen ggf. die Bezeichnungen von Komponenten-Deklaration und
    -Instanzierung nicht mit der Entity �berein.


Simulation
==========

In diesem Abschnitt werden wir das erstellte Design mit Hilfe der
Testbench simulieren.

-   Klicken Sie zum Starten des Simulators im "Flow Navigator" unter
    "Simulation" auf "Run Simulation"$\rightarrow$ "Run Behavioral
    Simulation". Hierbei wird nochmals eine Syntaxpr�fung des VHDL-Codes
    vorgenommen und auch beim Start des Simulators weitere Pr�fungen.
    Wenn hier noch Fehler auftreten, dann finden Sie in Vivado unten in
    den Fenstern "Tcl Console" oder "Messages" Hinweise dazu. Die
    Fehlermeldungen des Simulators werden in einer Log-Datei
    eingetragen: `intro1\intro1.sim\sim_1\behav\xsim\xvhdl.log`

    Öffnen Sie diese Datei mit einem Texteditor und versuchen Sie zu
    verstehen, wo Ihr Fehler liegt. Korrigieren Sie die Fehler im
    Quellcode und starten Sie den Simulator erneut. Wenn die Simulation
    korrekt gestartet ist, dann sollte in Vivado das Simulatorfenster
    geöffnet sein, wie in Abbildung  zu sehen ist.



-   Unter "Scope" sehen Sie den hierarchischen Aufbau Ihres Entwurfs.
    Auf der obersten Ebene liegt die Testbench "counter\_tb", darunter
    der Zähler "counter" (Instanzname "dut", siehe Instanzierung in der
    Testbench). Für das aktuell unter "Scope" ausgewählte Modul werden
    im Fenster "Objects" die jeweils definierten Ports, Signale und
    Konstanten aufgelistet; diese wiederum können per "Drag & Drop" in
    die Darstellung der Signalverläufe am rechten Fensterrand übernommen
    werden. Standardmässig werden die Signalverläufe aller Objekte der
    obersten Hierarchieebene dargestellt, in diesem Fall also der
    Testbench.

-   Rechts von "Scope" und "Objects" werden die Signalverläufe der
    ausgewählten Signale in einem "Wave Window" dargestellt.
    Standardmässig werden 1000 ns simuliert, diese Voreinstellung kann
    aber im "Flow Navigator" unter
    "Settings"$\rightarrow$\"'Simulation\"' vor dem nächsten Start des
    Simulators ge�ndert werden. Im "Waveform Display" finden Sie oben
    eine Leiste mit Kn�pfen zum Zoom und zu weiteren Funktionen. Um z.B.
    den kompletten Signalverlauf sehen zu können, nutzen Sie den "Zoom
    Fit"-Knopf.

-   Wenn Sie Signale aus dem "counter" sehen m�chten, dann Wählen Sie
    unter "Scope" den Instanznamen "dut" aus und können dann unter
    "Objects" entsprechende Signale mit der rechten Maustaste ausWählen:
    Im erscheinenden Kontext-Men� können Sie mit "Add To Wave Window"
    das Signal dann hinzufügen, wie in Abbildung
     zu sehen ist. Nun
    sehen Sie aber noch keine Signalverläufe für die neuen Signale.
    Hierzu müssen Sie den Simulator neu starten: Oberhalb des
    Simulatorfensters wurde die Vivado-Werkzeugleiste beim Start des
    Simulators erweitert. Dort finden Sie mehrere blaue Knöpfe, wenn Sie
    mit der Maus darüber fahren, wird die Funktion gezeigt.


    Mit dem "Restart"-Knopf können Sie den Simulator auf den Zeitpunkt 0
    zurücksetzen. Nun können Sie in der Werkzeugleiste eine neue
    Simulationszeit eingeben (z.B. 100 ns) und mit dem "Run for
    \..."-Knopf die entsprechende Zeit simulieren. Diese Kommandos
    können Sie auch unten in der Tcl-Konsole direkt eingeben (die bisher
    abgesetzten Kommandos werden auch dort angezeigt), dies ist meistens
    praktischer. Die Kommandos lauten: "restart" und "run 100 ns". Wenn
    Sie mehrfach "run \..." eingeben, wird entsprechend weiter
    simuliert, erst durch einen "restart" wird die Simulation wieder
    zur�ckgesetzt.

-   Wenn Sie im "Wave Window" mit der rechten Maustaste ein Signal
    ausWählen, dann können Sie im Kontextmenü unter "Radix" die
    Wertedarstellung ändern. Bei einem Zähler ist z.B. "unsigned
    decimal" vielleicht sinnvoll.

-   Die Einstellungen im "Wave Window" können Sie mit dem
    Diskettensymbol abspeichern: "Save Waveform Configuration". Diese
    Datei wird auf Wunsch (Abfrage im Dialog) dann zu Ihrem Projekt
    hinzugefügt und wenn Sie den Simulator wieder starten, sind die
    hinzugefügten Signale und alle Einstellungen gleich vorhanden.

-   Wenn Sie eine VHDL-Datei ändern müssen, brauchen Sie das
    Simulatorfenster nicht zu schliessen. Wählen Sie die betreffende
    Datei im jeweiligen Reiter im Simulatorfenster aus und f�hren Sie
    die �nderungen durch. Vergessen Sie nicht, anschlie�end die Datei zu
    speichern. Mit dem "Relaunch"-Knopf wird der Code erneut �bersetzt
    und die Simulation neu gestartet. Wichtig: Wenn Sie den VHDL-Code
    ändern und nur ein "Restart" machen, dann wird der alte Code
    simuliert! Daher muss nach jeder Code-�nderung ein "Relaunch"
    durchgeführt werden.

-   Wenn Sie fertig sind und die Simulation schliessen möchten, dann
    klicken Sie im blauen Titelbalken des Simulatorfensters ganz rechts
    auf das 'X'. Dies können Sie auch später mit den anderen Tools wie
    Synthese oder Implementierung so machen. Vivado ist eine integrierte
    Entwicklungsumgebung, welche verschiedene Tools wie
    Projektverwaltung, Simulator, Synthese oder Implementierung über den
    "Flow Navigator" startet und diese in einem integrierten Fenster
    anzeigt.

RTL-Analyse
===========

Mit Hilfe der so genannten RTL-Analyse können Sie sich einen Eindruck
von der Umsetzung des VHDL-Codes in Hardware machen. Dies ist noch nicht
der eigentliche Logiksynthese-Schritt, d.h. es findet noch keine
Abbildung auf die Zieltechnologie statt. Die Darstellung erfolgt hier
durch Makroblöcke. Allerdings ist diese Darstellung deutlich
�bersichtlicher im Vergleich zur eigentlichen Logik-Synthese, so dass
sie eine schnelle Beurteilung des VHDL-Codes erlaubt.

![[\[F11\]]{#F11 label="F11"}Vivado: Elaborated
Design](Bilder/viv11.eps){#F11}

Dr�cken Sie im "Flow Navigator" unter "RTL Analysis" auf "Open
Elaborated Design", bestätigen Sie den Hinweis mit "OK". Vivado startet
nun das Fenster "Elaborated Design" und im Reiter "Schematic" sollte nun
eine Schema-Zeichnung des elaborierten Codes zu sehen sein. Was Vivado
hier macht, ist im Grunde eine semantische Analyse Ihres VHDL-Codes und
zeigt Ihnen, wie der Code durch Vivado in Hardware umgesetzt wird. Sie
können in Abbildung [11](#F11){reference-type="ref" reference="F11"} das
Zählerregister "counter\_reg\[3:0\]" und den Inkrementer erkennen.
Ferner können Sie auch die beiden Flipflops "detect\_reg\[1:0\]" und das
UND-Gatter zur Flankenerkennung sehen. Damit können Sie �berpr�fen, ob
Ihr Code von Vivado so in Hardware umgesetzt wird, wie Sie sich das
vorgestellt haben. Schlie�en Sie das Fenster "Elaborated Design" wieder
durch Klick auf das 'X' im blauen Titelbalken.

Synthese
========

In diesem Abschnitt erfolgt die Logik-Synthese: Sie bildet den
VHDL-Entwurf auf die Bauelemente ab, die im FPGA zur Verf�gung stehen
(Flip-Flops, Look-Up-Tabellen, Multiplexer, RAM). Nebenbei kann die
Synthese auch Designfehler aufdecken, die in der Simulation evtl. nicht
sichtbar sind (z.B. unvollst�ndige Sensitivit�tslisten oder
unvollst�ndig auskodierte IF-Bedingungen). Es empfiehlt sich also
dringend, die Meldungen (Warnungen) der Synthese genau zu untersuchen.

-   Klicken Sie im "Flow Navigator" unter "Synthesis" auf "Run
    Synthesis". Im Dialog-Fenster ändern Sie nichts und quittieren mit
    "OK". Der Syntheselauf wird etwas Zeit ben�tigen, Sie können im
    Vivado-Fenster ganz unten unter "Design Runs" sehen, wie weit die
    Synthese ist.

-   Entsprechende Warnungen oder Fehler werden wieder im Reiter
    "Messages" angezeigt. �berpr�fen Sie bitte, dass keine
    unvollst�ndigen Sensitivit�tslisten oder Latches durch
    unvollst�ndige Signalzuweisungen vorliegen. Dies sind keine Fehler
    sondern nur Warnungen, d.h. Ihr Code wird in Hardware umgesetzt,
    aber m�glicherweise können Sie hierdurch eine Hardware erhalten, die
    nicht korrekt funktioniert.

-   Sofern der Entwurf noch Fehler (z.B. Syntaxfehler) enth�lt, wird die
    Synthese abgebrochen. Korrigieren Sie in diesem Fall den Fehler,
    f�hren Sie erst wieder eine Simulation durch und starten Sie dann
    wieder die Synthese.

-   Wenn der Syntheselauf fertig ist, dann �ffnet sich ein Fenster.
    Wählen Sie dort "View Reports" aus. Im Vivado-Fenster wird unten nun
    der Reiter "Reports" hervorgehoben. Mit einem Doppelklick auf die
    Reports werden diese in einem Editor-Fenster geöffnet. Abbildung
    [12](#F12){reference-type="ref" reference="F12"} zeigt einen
    Ausschnitt aus dem "Utilization Report", in welchem zu erkennen ist,
    dass 4 Slice-LUTs und 6 Flipflops für die Implementierung des
    Designs ben�tigt wird.

    ![[\[F12\]]{#F12 label="F12"}Utilization Report
    ](Bilder/viv12.eps){#F12}

-   Klicken Sie im "Flow Navigator" unter "Synthesis" auf "Open
    Synthesized Design". Wenn Sie nun auf "Schematic" klicken, dann wird
    ein Fenster mit einer Schema-Zeichnung der Umsetzung Ihres Codes in
    die Zieltechnologie angezeigt, wie Abbildung
    [13](#F13){reference-type="ref" reference="F13"} zeigt.

    ![[\[F13\]]{#F13 label="F13"} Synthesis Schematic
    ](Bilder/viv13.eps){#F13}

-   Schlie�en Sie das Fenster "Synthesized Design" wieder durch Klick
    auf das 'X' im blauen Titelbalken.

Definition der Randbedingungen
==============================

In diesem Abschnitt sollen die "Constraints" (dt.: Randbedingungen) für
das Design definiert werden. Die Randbedingungen sind für die
Implementierung notwendig. Was auf jeden Fall ben�tigt wird, sind die
Zuordnung der Ports des VHDL-Codes zu den Pins des FPGA. Ferner kann man
auch (und sollte man auch) zeitliche Randbedingungen angeben. Im
einfachsten Fall ist dies die Angabe der maximalen Taktfrequenz, die das
Design erreichen können soll.

-   Sofern das synthetisierte Design ("Synthesized Design") nicht
    geöffnet ist, klicken Sie im "Flow Navigator" unter "Synthesis" auf
    "Open Synthesized Design".

-   Wählen Sie in der Werkzeugleiste statt dem "Default Layout" für
    Vivado das Layout "I/O Planning". Sie m�ssten dann eine Ansicht wie
    in Abbildung [14](#F14){reference-type="ref" reference="F14"}
    bekommen.

    ![[\[F14\]]{#F14 label="F14"} Vivado: I/O-Planning Ansicht
    ](Bilder/viv14.eps){#F14}

    ![[\[F15\]]{#F15 label="F15"} I/O Planning: I/O Ports
    ](Bilder/viv15.eps){#F15}

-   Unter dem Reiter "I/O Ports" ganz unten können die Eigenschaften der
    I/O-Ports definiert werden, wie in Abbildung
    [15](#F15){reference-type="ref" reference="F15"} gezeigt. Hier
    erfolgt die Zuordnung der Ports zu den FPGA-Pins ("Site"), die
    Einstellung der I/O-Standards und weitere Merkmale, wie
    Treiberst�rke oder die "Slew Rate". Erweitern Sie die Eintr�ge
    "ctr\_out" und "Scalar ports" durch Klicken auf das Pfeil-Zeichen.
    Geben Sie nun die "Sites" oder FPGA-Pins wie in Abbildung
    [15](#F15){reference-type="ref" reference="F15"} gezeigt für die
    entsprechenden Ports ein. Dies entspricht einer Zuordnung für das
    Basys3-Board, so dass die Ausgabe des Zähler auf den ersten vier
    LEDs erfolgt, zwei der Taster benutzt werden und der Taktoszillator
    angeschlossen wird. Weitere Informationen dazu können Sie dem
    "Reference Manual" für das Board entnehmen, sofern Sie das Board
    verwenden m�chten. Wichtig ist noch unter "I/O Std" den
    "LVCMOS33"-Standard einzustellen, da LEDs, Taster und der
    Clock-Oszillator auf dem Board mit 3,3 V arbeiten. Die restlichen
    Einstellungen können Sie auf ihren Default-Werten belassen.

    ![[\[F16\]]{#F16 label="F16"} Timing Constraints: Taktfrequenz
    ](Bilder/viv16.eps){#F16}

-   Speichern Sie die Randbedingungen mit "File" $\rightarrow$
    "Constraints" $\rightarrow$ "Save". Best�tigen Sie die Meldung, dass
    die Syntheseergebnisse anschlie�end m�glicherweise veraltet sein
    werden, mit OK. Geben Sie der Datei im folgenden Dialog den Namen
    "constr.xdc".

-   Zur Definition der Randbedingung für den Systemtakt klicken Sie im
    "Flow Navigator" unter "Synthesis" auf "Constraints Wizard". Klicken
    Sie im Startfenster des "Timing Constraints Wizard" einmal auf
    "Next". Im folgenden Dialog können Sie die Frequenz des Taktsignals
    "clk" eingeben (Abbildung [16](#F16){reference-type="ref"
    reference="F16"}). Tragen Sie unter "Recommended Constraints"
    $\rightarrow$ "Frequency (MHz)" den Wert 100 ein (die Frequenz des
    Taktoszillators betr�gt auf dem BASYS3-Board 100 MHz).

-   �berspringen Sie alle weiteren Dialogfenster mit "Skip to Finish"
    und schlie�en Sie auch den letzten Dialog mit Finish.

-   Beenden Sie das Definieren von Constraints durch einen Klick auf das
    Kreuz rechts oben im blauen Titelbalken von "Synthesized Design".

-   Im "Project Manager"-Fenster sehen Sie unter "Sources" $\rightarrow$
    "Constraints" die Datei "constr.xdc". Öffnen Sie die Datei mit einem
    Doppelklick. Die Datei enth�lt Kommandos zum Erzeugen der oben
    definierten Constraints. Eine solche Datei k�nnte also auch von Hand
    erstellt und mit "Add Sources" zum Projekt hinzugef�gt werden. Sie
    können auch �nderungen direkt hier in der Datei vornehmen, falls
    z.B. die Pinzuordnung nicht korrekt ist.

-   Hinweis: Nach dieser Vorgehensweise wird zun�chst ein Syntheselauf
    durchgef�hrt, ohne dass Timing-Constraints vorher definiert wurden.
    Dabei werden bei der Synthese dann globale Optimierungsziele
    vorgegeben (siehe Einstellungen der Synthese). Nimmt man nach der
    Synthese eine Timing-Analyse vor (analog zur weiter unten
    beschriebenen Vorgehensweise nach der Implementierung), so wird
    gemeldet, dass keine Constraints vorliegen (Eintrag "Check Timing").
    Man kann dann allerdings nach Eingeben der Timing-Constraints
    nochmals einen Syntheselauf durchf�hren, wobei dann die
    Timing-Constraints ber�cksichtigt werden (und bei allen weiteren
    Synthesel�ufen). Dabei ist allerdings zu beachten, dass insbesondere
    die Verz�gerungszeiten durch die Verdrahtung ("net delay") nur
    gesch�tzt werden (siehe [@Kesel09]).

Implementierung
===============

In diesem Abschnitt wird durch die "Implementation" (dt.:
Implementierung) das Platzieren der Bauelemente auf dem FPGA vorgenommen
und diese anschlie�end verdrahtet. Diesen Vorgang nennt man auch
"Place&Route".

-   Klicken Sie im "Flow Navigator" unter "Implementation" auf "Run
    Implementation". Wenn der Hinweis kommt, dass die Synthese
    "out-of-date" ist, dann klicken Sie auf "Yes". Damit wird der
    Syntheselauf nochmals ausgef�hrt. Grunds�tzlich ist es so, dass
    Vivado die Zeitstempel der Quelldateien (VHDL-Codes, Constraints,
    etc.) �berpr�ft und die entsprechenden Tools, die davon betroffen
    sind, wieder startet und auch alle Schritte vorher wieder ausf�hrt.
    Im Grunde reicht es daher aus, wenn Sie sp�ter Quellen ver�ndert
    haben, dass Sie einfach wieder auf den letzten Schritt "Generate
    Bitstream" klicken, so dass damit dann auch alle notwendigen
    vorherigen Schritte, wie Synthese und Implementierung, wieder
    ausgef�hrt werden.

-   Klicken Sie nach dem Abschluss der Implementierung auf "View
    Reports" und �berpr�fen Sie die Meldungen im Reiter "Messages". Um
    zu �berpr�fen, ob die von Ihnen vorgegebenen "Timing Constraints"
    eingehalten werden konnten, können Sie im Reiter "Reports" nach
    "Route Design" suchen und dort den "Timing Summary Report" Öffnen.
    Eine schnelle �bersicht bekommen Sie auch weiter oben im Fenster
    "Project Summary".

-   Öffnen Sie nun das implementierte Design, indem Sie auf "Open
    Implemented Design" klicken. Es �ffnet sich unten ein weiterer
    Reiter "Timing" in welchem Sie detaillierte Informationen zum Timing
    erhalten (Abbildung [17](#F17){reference-type="ref"
    reference="F17"}). Wenn Sie in Abbildung
    [17](#F17){reference-type="ref" reference="F17"} im
    Navigationsfenster links den Punkt "Intra Clock Paths $\rightarrow$
    clk" aufklappen, können Sie detailliertere Informationen zu den
    kritischen Pfaden erhalten. Unter "Setup" finden Sie die
    Informationen zu den Pfaden, die von Register zu Register (oder ggf.
    vom Eingang zum Register, falls Constraints für die Eing�nge
    spezifiziert wurden) gehen. Ein positiver "Slack" bedeutet dabei,
    dass die Constraints eingehalten wurden (siehe auch [@Kesel09]).
    Weitere Informationen sind in der jeweiligen Zeile dann zu finden:
    Start- und Endpunkt des Pfades, Gesamtverz�gerung und die Anteile
    des Routings und der Komponenten. Unter "Requirement" sollten Sie
    die von Ihnen vorgegebenen Constraints sehen. Wenn Sie mit der Maus
    eine Zeile ausWählen, dann können Sie mit der rechten Maustaste ein
    Kontextmen� aufrufen: Dort können Sie z.B. mit "Schematic" sich den
    Pfad im Schema-Editor anzeigen lassen oder mit "View Path Report"
    detaillierte Informationen zu diesem Pfad anzeigen lassen.

    ![[\[F17\]]{#F17 label="F17"} Timing Report
    ](Bilder/viv17.eps){#F17}

Erstellen des Bit-Files und Konfiguration des FPGAs
===================================================

Bevor das FPGA konfiguriert werden kann, m�ssen die Ergebnisse des
Implementierungslaufs in einen "Bitstream" (eine Konfigurationsdatei für
das FPGA) umgewandelt werden. Die nachfolgenden Schritte sind nur
sinnvoll, wenn Sie ein FPGA-Board zur Verf�gung haben.

-   Klicken Sie im "Flow Navigator" unter "Program and Debug" auf
    "Generate Bitstream" und warten Sie, bis die Bitstream-Generierung
    beendet ist.

-   Wenn Sie nun ein Basys3-Board (oder ein anderes) zur Verf�gung
    haben, dann verbinden Sie es mit einem USB-Kabel mit dem
    Entwicklungsrechner.

    ![[\[F18\]]{#F18 label="F18"} Hardware Manager
    ](Bilder/viv18.eps){#F18}

-   Wenn die Generierung des Bitstream fertig ist, können Sie auf "Open
    Hardware Manager" klicken. In der linken oberen Ecke des Fensters
    "Hardware Manager" (siehe Abbildung [18](#F18){reference-type="ref"
    reference="F18"}) klicken Sie auf "Open Target" $\rightarrow$ "Auto
    Connect". Stellen Sie vorher sicher, dass das Board eingeschaltet
    ist!

-   Nun m�ssten Sie die Ansicht wie in Abbildung
    [19](#F19){reference-type="ref" reference="F19"} sehen. Wenn Sie nun
    auf "Program Device" klicken, erhalten Sie den FPGA-Baustein zur
    Auswahl und danach die voreingestellte Auswahl des Bitstream-Files.
    Klicken Sie hier auf "Program device". Das FPGA wird nun
    programmiert. Wenn dieser Vorgang beendet ist, m�sste Ihr Design zum
    Test auf dem Board bereit sein!

    ![[\[F19\]]{#F19 label="F19"} Hardware Manager: Programmierung des
    Targets ](Bilder/viv19.eps){#F19}

-   Hinweis: Wenn bei "Program Device" kein Bit-File erscheint, dann ist
    es in der Regel so, dass Sie m�glicherweise beim Erstellen des
    Vivado-Projekts den falschen FPGA-Typ ausgew�hlt haben. Bei der
    Verbindung des Hardware Managers zum FPGA wird die Typbezeichnung
    ausgelesen (Fenster "Hardware Device Properties"). Wenn diese nicht
    mit der Typbezeichnung im Bit-File �bereinstimmt, wird das Bit-File
    nicht zum Download automatisch eingetragen. Sollte dies passiert
    sein, dann m�ssen Sie im Project Manager auf "Settings" gehen und
    dort den FPGA-Typ korrigieren ("xc7a35tcpg236-1") und dann alle
    Schritte nochmals durchlaufen, indem Sie einfach auf "Generate
    Bitstream" klicken.
