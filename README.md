# JUEGO-PONG-SOCKET-TCP
Juego clásico del (PING PONG) entre un servidor y dos clientes(los jugadores ) 

1) Descargar los archivos cliente.cpp y servidor.cpp.

2) En el primer terminal ejecutar el comando "g++ servidor.cpp -o servidor -pthread" , y luego "./servidor". 
  Y el servidor quedará a las espera de que los jugadores(clientes) se conecten.
  
3) En las dos terminales restantes hay que ejecutar el juego con el comando "g++ cliente.cpp -o cliente -pthread" 
  y "./cliente" para lograr la conexión al servidor, y se ejecute el juego en ambas terminales.
  
4) Ahora que la conexión se realizó de forma exitosa. se puede simular un juego de PONG de 3 partidas.
