INFORME HAMBURGUESAS:
En este uinforme coloque mi pseudocodigo explicando que hace cada funcion y como se corresponde una con otra.

void* imprimirAccion(void *data, char *accionIn) {
en esta funcion lo que hacemos es abrir el  txt para poder guardar la salida de mi programa

void* cortar(void *data) {
en esta función cortamos el ajo, la cebolla y el perejil y luego mandamos la señal para que el equipo vaya a la siguiente acción que es mezclar

void* mezclar(void *data) {
aquí esperamos la señal y luego mezclamos los ingredientes con la carne picada, luego mandamos la función para salar

void* salar(void *data) {
aquí lo que tenemos es un semáforo usado como mutex que lo que hace es preguntar si alguien esta usando el salero, si hay un equipo que lo esta usando, el otro equipo no lo va a poder usar, una vez que el equipo termino de usar el salero, lo libera y manda la señal para que el equipo vaya a la función armar medallones

void* armarmedallones(void *data) {
en esta función armamos los medallones esperando la señal de salar y luego habilitamos la señal para ir a la función cocinar

void* cocinar(void *data) {
en esta función lo que primero hacemos es con un mutex fijarnos si algún equipo esta usando la plancha, si nadie la esta usando un equipo la utiliza, cocina la hamburguesa y luego libera la plancha, luego mandamos la señal para que el equipo se vaya a hornear el pan

void* hornear(void *data) {
en esta función también tenemos un mutex donde verificamos si algún equipo esta utilizando el horno para el pan, de no ser asi el equipo utiliza el horno y luego lo libera para que el otro lo pueda utilizar, luego damos la señal para irnos a cortar el tomate y la lechuga

void* cortarextras(void *data) {
en esta función esperamos la señal de hornear y cortamos la lechuga y el tomate, luego damos la señal a armar la hamburguesa

void* armarhamburguesa(void *data) {
en esta función esperamos la señal de cortar los extras para que podamos armar la hamburguesa, aquí armamos la hamburguesa final, luego el equipo al terminar de armar la hamburguesa le sumamos uno a mi int final, que inicialice al principio de todo y le preguntamos si es igual a uno, si esto se cumple, imprimimos al equipo ganador, si final no es igual a 1, imprimimos a los equipos perdedores.

void* ejecutarReceta(void *i){
aquí lo que hacemos además de todas las cosas que ya venían implementadas, ponemos los semáforos e hilos que faltan, luego abrimos el txt de la salida escribimos alli sobre las acciones de los equipos 
 Luego abrimos la receta y recorro las acciones que son iguales a la cantidad de lineas de la receta donde obtengo  cada una de las líneas y las voy dividiendo con strtok, luego guarda la acción, luego voy recorriendo las palabras de cada línea y luego le paso los ingredientes al struct, al final cerramos la receta.
Luego seguimos completando las cosas que dejo la profe como el seteo de semáforos, inicializar los semáforos, crear los demás hilos, luego destruir los semáforos.
Luego en el MAIN:
Inicializo los semáforos en 1 y luego al final los destruyo  

