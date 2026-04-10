# GramofonController

Embedded sistem za upravljanje gramofonom sa detekcijom senzora, više režima rada i LCD prikazom.

🔌 Ulazi / Izlaz

 `P0_1` – detekcija nitne
 `P0_2` – detekcija kruga
 `P2_0` – start sistema (motor)
 
⚙️ Stanja

 `STOP`
 `READY`
 `BROJANJE`
 `POMERANJE`

📡 Serijska komunikacija

Prioritet:
```
STOP → START → IZBOR MODA
```

🧩 Moduli

 `display` – LCD prikaz
 `gpio` – upravljanje pinovima 
 `timer1` – merenje vremena
 `serijska` – UART komunikacija
 `gramofon` – logika sistema

