/*#include "GridNode.h"
#include "ListaGridNodos.h"


//Aca empieza el algoritmo para calcular el camino mas corto
   // -> A star <-


// Función para buscar el camino usando el algoritmo A*
ListaGridNodos* aStarPathfinding(GridCell** grid, int gridSize, GridNode* inicio, GridNode* fin) {
    // Crear la lista del camino mas corto
    ListaGridNodos* path = new ListaGridNodos();

    // Crear listas abierta y cerrada
    ListaGridNodos* listaAbierta = new ListaGridNodos();
    ListaGridNodos* listaCerrada = new ListaGridNodos();
    
    // Agregar el nodo de inicio a la lista abierta
    listaAbierta->agregarInicio(inicio);

    int ejecucionDelWhile = 1;
    while (!listaAbierta->esVacia()) {
        std::cout << "-----Ves que el While se esta ejecutando: " << ejecucionDelWhile << std::endl;

        // Encontrar el nodo con el valor f mínimo en la lista abierta
        int fminimo = listaAbierta->getFporPos(0);
        int hminimo = listaAbierta->getHporPos(0);
        int indiceMinimo = 0;

        std::cout << "Informacion de lista Abierta: " << std::endl; /// comprobacion
        for (int i = 0; i < listaAbierta->cant; ++i) {
            // Imprimir el nodo de la posición 'i'
            
            listaAbierta->imprimirNodo(i);

            // Obtener el valor de f de la posición 'i'
            int f = listaAbierta->getFporPos(i);

            // Encontrar el f mínimo y actualizar su posición
            if (f < fminimo) {
                fminimo = f;
                hminimo = listaAbierta->getHporPos(i);
                indiceMinimo = i;
            } else if (f == fminimo) {
                // En caso de empate en el valor f, se compara el valor h
                int h = listaAbierta->getHporPos(i);
                if (h < hminimo) {
                    hminimo = h;
                    indiceMinimo = i;
                }
            }
        }

        // Corroborar los resultado de la eleccion de lo anterior
        std::cout << "EL indice Minimo elegido fue: " << indiceMinimo << std::endl; 
        std::cout << "-Cantidad Nodos de lista abierta: " << listaAbierta->cant << std::endl;
        std::cout << "El f minimo fue de: " << fminimo << std::endl;        

        // Obtener el nodo con el valor f mínimo sacandolo de la lista abierta
        
        GridNode* nodoActual = listaAbierta->sacarNodo(indiceMinimo);


        // Verificar si se ha llegado al objetivo
        if (nodoActual->x == fin->x && nodoActual->y == fin->y) {
            std::cout << "Se llego a la meta" << std::endl;
            // Reconstruir el camino desde el objetivo hasta el inicio
            GridNode* temp = nodoActual->padre;
            while (temp->padre != nullptr) {
                temp->imprimirNodo();
                path->agregarInicio(temp);
                temp = temp->padre;
            }
            return path;
            break;
        }
        
        // Parte en donde se generan los vecinos
        std::cout << "--CREACION DE VECINOS--" << std::endl;

        // Generar vecinos del nodo actual
        ListaGridNodos* vecinos = new ListaGridNodos();
        std::cout <<" VECINO CALCULADO" <<std::endl;
        CalcularVecinos(vecinos, grid,nodoActual , fin, gridSize);

        // Asignacion de las posiciones permitidas a la lista
        std::cout << "SALTO DE LINEA, MOSTRAR LA LISTA VECINOS" << std::endl;
        std::cout << "---Cantidad Nodos de lista VECINOS: " << vecinos->cant << std::endl;
        for (int i = 0; i < vecinos->cant; i++)
        {
            vecinos->imprimirNodo(i);
        }
        
        
        // Calcular valores g, h y f para cada vecino y agregarlos a la lista abierta
        int cantVecinos = vecinos->cant;
        int iter = 0;
        while (iter < vecinos->cant && vecinos->inicio!=nullptr)        
        {
            // Verificar si el vecino ya está en la lista cerrada
            int posCerrada = listaCerrada->encontrarPos(vecinos->getXporPos(iter), vecinos->getYporPos(iter));
            std::cout<< "valor pos cerrada " << posCerrada << std::endl;
            if (posCerrada != -1)
            {
                // Verificar si el nuevo camino hacia el vecino es más largo
                if (vecinos->getGporPos(iter) >= listaCerrada->getGporPos(posCerrada)) {
                    vecinos->eliminarPos(iter);
                }
                // Caso sea el vecino menor que el de la lista Cerrada, se elimina el Nodo de lista Cerrada
                else {
                    listaCerrada->eliminarPos(posCerrada);
                }
            }

            // Verificar si el vecino ya está en la lista abierta
            int posAbierta = listaAbierta->encontrarPos(vecinos->getXporPos(iter), vecinos->getYporPos(iter));
            std::cout<< "valor pos abierta " << posAbierta << std::endl;
            if (posAbierta != -1)
            {
                // Verificar si el nuevo camino hacia el vecino es más largo
                if (vecinos->getGporPos(iter) >= listaAbierta->getGporPos(posAbierta)) {
                    vecinos->eliminarPos(iter);
                }
                // Caso sea el vecino menor que el de lista Abierta, se elimina el Nodo de lista Abierta
                else {
                    listaAbierta->eliminarPos(posAbierta);
                }
            }

            // Si no se elimino un vecino, se avanza en uno el iter
            if (cantVecinos == vecinos->cant)
            {
                // Establecer el nodo actual como el padre del vecino
                vecinos->agregarPadre(iter, nodoActual);
                iter++; // sumar en uno el iter
            }
            // Caso que si han eliminado vecinos, solo actualizar la variable cantVecinos
            else
            {
                cantVecinos = vecinos->cant;
            }
        }
        //Verificar que vecinos no sea vacio
        if (!vecinos->esVacia()){
            // Agregar el vecino a la lista abierta
            listaAbierta->agregarListAlFinal(vecinos);
            delete vecinos;
            listaCerrada->agregarInicio(nodoActual);
            // Iteracion de la cantidad de ejecuciones del while
            ///delete nodoActual;
        }
        ejecucionDelWhile++;
    }
    return listaCerrada;
}


void CalcularVecinos(ListaGridNodos* vecinos, GridCell** grid, GridNode* nodoActual ,GridNode* fin, int gridSize)
{
    // Diagonal izquierdo superior
    if (nodoActual->x-1 >= 0 && nodoActual->y-1 >= 0)
    {
        if (!grid[nodoActual->y-1][nodoActual->x-1].bloqueado)
        {
            GridNode* temp = new GridNode(nodoActual->x-1, nodoActual->y-1);
            temp->calcularDistancia(nodoActual, fin);
            vecinos->agregarInicio(temp);
        }
    }
    // Arriba
    if (nodoActual->y-1 >= 0 && !grid[nodoActual->y-1][nodoActual->x].bloqueado)
    {
        GridNode* temp = new GridNode(nodoActual->x, nodoActual->y-1);
        temp->calcularDistancia(nodoActual, fin);
        vecinos->agregarInicio(temp);
    }
    // Diagonal derecho superior
    if (nodoActual->x+1 < gridSize && nodoActual->y-1 >= 0)
    {
        if (!grid[nodoActual->y-1][nodoActual->x+1].bloqueado)
        {
            GridNode* temp = new GridNode(nodoActual->x+1, nodoActual->y-1);
            temp->calcularDistancia(nodoActual, fin);
            vecinos->agregarInicio(temp);
        }
    }
    // Izquierda
    if (nodoActual->x-1 >= 0 && !grid[nodoActual->y][nodoActual->x-1].bloqueado)
    {
        GridNode* temp = new GridNode(nodoActual->x-1, nodoActual->y);
        temp->calcularDistancia(nodoActual, fin);
        vecinos->agregarInicio(temp);
    }
    // Derecha
    if (nodoActual->x+1 < gridSize && !grid[nodoActual->y][nodoActual->x+1].bloqueado)
    {
        GridNode* temp = new GridNode(nodoActual->x+1, nodoActual->y);
        temp->calcularDistancia(nodoActual, fin);
        vecinos->agregarInicio(temp);
    }
    // Diagonal izquierdo inferior
    if (nodoActual->x-1 >= 0 && nodoActual->y+1 < gridSize)
    {
        if (!grid[nodoActual->y+1][nodoActual->x-1].bloqueado)
        {
            GridNode* temp = new GridNode(nodoActual->x-1, nodoActual->y+1);
            temp->calcularDistancia(nodoActual, fin);
            vecinos->agregarInicio(temp);
        }
    }
    // Abajo
    if (nodoActual->y+1 < gridSize && !grid[nodoActual->y+1][nodoActual->x].bloqueado)
    {
        GridNode* temp = new GridNode(nodoActual->x, nodoActual->y+1);
        temp->calcularDistancia(nodoActual, fin);
        vecinos->agregarInicio(temp);
    }
    // Diagonal derecho inferior
    if (nodoActual->x+1 < gridSize && nodoActual->y+1 < gridSize)
    {
        if (!grid[nodoActual->y+1][nodoActual->x+1].bloqueado)
        {
            GridNode* temp = new GridNode(nodoActual->x+1, nodoActual->y+1);
            temp->calcularDistancia(nodoActual, fin);
            vecinos->agregarInicio(temp);
        }
    }
}
*/