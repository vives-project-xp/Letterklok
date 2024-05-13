<h1>Software</h1>
<h2> PlatformIO </h2>
<p> Het programma voor dit project werd geschreven in C++. Aangezien we gebruik maken van leds die we wensen aan te sturen, gebruiken we verschillende externe libraries, waaronder de bekende Adafruit_Neopixel library. Als we deze library willen gebruiken, hebben we nood aan de PlatformIO extensie.
<img src = "../Foto's/neopixel.png">
<img src = "..Foto's/menu.png">
<p> Eenmaal dit geïnstalleerd is, maak je een nieuw project aan. Geef het een naam, selecteer het bordje (ESP32-c3 Devkit M1) en het framework (Arduino).
<img src = "../Foto's/platformioproject.png">
<img src = "../Foto's/platformfinal.png">
Vervolgens importeren we de Adafruit_Neopixel library via het PIO menu; links kies je de optie 'libraries' en in de zoekbalk typ je de naam. </p>

Verder voeg je ook nog NTPClient en WiFiUdp toe, op dezelfde manier.

<h2> Main File </h2>
We kaarten de belangrijkste punten uit de code even aan:

- Er wordt verbinding met WiFi gemaakt, waarna de TimeClient, buttons en de leds worden geïnitialiseerd.
- De huidige tijd wordt opgehaald via een database op het internet, die tijd wordt dan vervolgens omgezet naar binary code en dan, door middel van een for loop, op de ledstrips gezet.
- Functionaliteit van de buttons worden daarna beschreven door middel van een reeks if-statements. Hier worden de kleurwaarden ook aangepast, naargelang welke knop actief wordt.
- Een test programma is inbegrepen, waarbij alle leds tegelijk aangestuurd worden.



[MAIN README](../README.md)

