# Componenten

# 1: Voeding
<img src = "voeding.jpg" height = 300></img>

<p>Ingangsspanning: 90 - 264V AC<br>
Uitgangsspanning: 5V DC<br>
Uitgangsstroom max: 5A<br>
Vermogen: 25W<br>

We plaatsen twee exemplaren in parallel om onze leds en controller te voeden.<br>
We houden in gedacht dat onze leds nooit allemaal op hetzelfde moment zullen oplichten, dus dit is meer dan voldoende. </p>


# 2: Controller
<img src = "controller.webp" height = 300></img>

<p>Om onze leds aan te sturen, maken we gebruik van de ESP32-C3, een controller waarmee we reeds vertrouwd zijn. 

Dit toestel werkt op een spanning van 3.3V, heeft een USB-interface en 22 GPIO-pinnen wat meer dan genoeg is voor dit project.


# 3: Level Shifter
<img src = "level_shifter.webp" height = 300></img>

Omdat de spanning van onze controller (3.3V) verschilt met de spanning waarop onze LEDs werken (5V), hebben we level shifters nodig om de spanningen juist om te zetten.

Deze level shifter kan omzettingen doen tussen:<br>
*1.8V<br>
*2.8V<br>
*3.3V<br>
*5V<br>



# 4: Ledstrip
<img src = "ledstrip.jpg" height = 300></img>