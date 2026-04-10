# GramofonController

Embedded system for controlling a gramophone, featuring sensor detection, multiple operating modes, and an LCD display.

🔌 Inputs / Outputs

`P0_1` – Rivet detection  
`P0_2` – Circle detection  
`P2_0` – System start (motor)
 
⚙️ States

 `STOP`
 `READY`
 `BROJANJE`
 `POMERANJE`

📡 Serial Communication

Priority:
```
STOP → START → IZBOR MODA
```

🧩 Modules

`display` – LCD display handling  
`gpio` – Pin control  
`timer1` – Time measurement  
`serial` – UART communication  
`gramophone` – Core system logic  

