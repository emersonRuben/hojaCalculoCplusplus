#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <limits>
#include <cstdlib>
#include <sstream>

class HojaCalculo {
private:
    std::vector<std::vector<double>> celdas;

public:
    void agregarFila() {
        std::vector<double> nuevaFila(celdas.empty() ? 1 : celdas[0].size(), 0.0);
        celdas.push_back(nuevaFila);
    }

    void eliminarFila(size_t index) {
        if (index < celdas.size()) {
            celdas.erase(celdas.begin() + index);
        } else {
            throw std::out_of_range("Indice de fila fuera de rango");
        }
    }

    void agregarColumna() {
        for (auto& fila : celdas) {
            fila.push_back(0.0);
        }
    }

    void eliminarColumna(size_t index) {
        if (!celdas.empty() && index < celdas[0].size()) {
            for (auto& fila : celdas) {
                fila.erase(fila.begin() + index);
            }
        } else {
            throw std::out_of_range("Indice de columna fuera de rango");
        }
    }

    void actualizarCelda(size_t fila, size_t columna, double valor) {
        if (fila < celdas.size() && columna < celdas[fila].size()) {
            celdas[fila][columna] = valor;
        } else {
            throw std::out_of_range("Indice de celda fuera de rango");
        }
    }

    double obtenerCelda(size_t fila, size_t columna) const {
        if (fila < celdas.size() && columna < celdas[fila].size()) {
            return celdas[fila][columna];
        } else {
            throw std::out_of_range("Indice de celda fuera de rango");
        }
    }

    double operarCeldas(size_t fila1, size_t col1, size_t fila2, size_t col2, char operacion) const {
        double valor1 = obtenerCelda(fila1, col1);
        double valor2 = obtenerCelda(fila2, col2);

        switch (operacion) {
            case '+': return valor1 + valor2;
            case '-': return valor1 - valor2;
            case '*': return valor1 * valor2;
            case '/':
                if (valor2 != 0) return valor1 / valor2;
                else throw std::invalid_argument("Error: Division por cero.");
            default:
                throw std::invalid_argument("Error: Operacion no valida.");
        }
    }

    double operarFila(size_t fila, char operacion) const {
        if (fila >= celdas.size()) {
            throw std::out_of_range("Indice de fila fuera de rango");
        }

        double resultado = celdas[fila][0];
        for (size_t col = 1; col < celdas[fila].size(); ++col) {
            switch (operacion) {
                case '+': resultado += celdas[fila][col]; break;
                case '-': resultado -= celdas[fila][col]; break;
                case '*': resultado *= celdas[fila][col]; break;
                case '/':
                    if (celdas[fila][col] != 0) resultado /= celdas[fila][col];
                    else throw std::invalid_argument("Error: Division por cero.");
                default: throw std::invalid_argument("Error: Operacion no valida.");
            }
        }
        return resultado;
    }

    double operarColumna(size_t columna, char operacion) const {
        if (columna >= celdas[0].size()) {
            throw std::out_of_range("Indice de columna fuera de rango");
        }

        double resultado = celdas[0][columna];
        for (size_t fila = 1; fila < celdas.size(); ++fila) {
            switch (operacion) {
                case '+': resultado += celdas[fila][columna]; break;
                case '-': resultado -= celdas[fila][columna]; break;
                case '*': resultado *= celdas[fila][columna]; break;
                case '/':
                    if (celdas[fila][columna] != 0) resultado /= celdas[fila][columna];
                    else throw std::invalid_argument("Error: Division por cero.");
                default: throw std::invalid_argument("Error: Operacion no valida.");
            }
        }
        return resultado;
    }

    void mostrar() const {
        std::cout << "Hoja de Cálculo:\n";

        for (size_t i = 0; i < celdas[0].size(); ++i) {
            std::cout << "-------";
        }
        std::cout << std::endl;

        for (const auto& fila : celdas) {
            std::cout << "|";
            for (const auto& celda : fila) {
                std::cout << " " << celda << " |";
            }
            std::cout << std::endl;

            for (size_t i = 0; i < celdas[0].size(); ++i) {
                std::cout << "-------";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void guardarCSV(const std::string& nombreArchivo) const {
        std::ofstream archivo(nombreArchivo);
        if (archivo.is_open()) {
            for (const auto& fila : celdas) {
                for (size_t i = 0; i < fila.size(); ++i) {
                    archivo << fila[i];
                    if (i < fila.size() - 1) {
                        archivo << ",";
                    }
                }
                archivo << "\n";
            }
            archivo.close();
        } else {
            std::cerr << "No se pudo abrir el archivo para guardar." << std::endl;
        }
    }

    void cargarCSV(const std::string& nombreArchivo) {
        std::ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            std::cerr << "No se pudo abrir el archivo para cargar." << std::endl;
            return;
        }

        celdas.clear(); // Limpiar celdas antes de cargar
        std::string linea;
        while (std::getline(archivo, linea)) {
            std::vector<double> fila;
            std::stringstream ss(linea);
            std::string valor;
            while (std::getline(ss, valor, ',')) {
                fila.push_back(std::stod(valor));
            }
            celdas.push_back(fila);
        }

        archivo.close();
        std::cout << "Archivo CSV cargado correctamente." << std::endl;
    }
};

double leerNumero() {
    double numero;
    while (true) {
        std::cin >> numero;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida. Por favor, ingrese un numero: ";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return numero;
        }
    }
}

size_t leerTamano(const std::string& mensaje) {
    size_t tamano;
    while (true) {
        std::cout << mensaje;
        std::cin >> tamano;
        if (std::cin.fail() || tamano < 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida. Por favor, ingrese un numero entero positivo: ";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return tamano;
        }
    }
}

void limpiarConsola() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void menu(HojaCalculo& hoja) {
    int opcion;
    do {
        limpiarConsola();
        std::cout << "--- Menu de Hoja de Calculo ---\n";
        std::cout << "1. Agregar Fila\n";
        std::cout << "2. Eliminar Fila\n";
        std::cout << "3. Agregar Columna\n";
        std::cout << "4. Eliminar Columna\n";
        std::cout << "5. Actualizar Celda\n";
        std::cout << "6. Agregar Valor a Celda Especifica\n";
        std::cout << "7. Realizar Operacion Aritmetica entre Celdas\n";
        std::cout << "8. Operar Todos los Elementos de una Fila\n";
        std::cout << "9. Operar Todos los Elementos de una Columna\n";
        std::cout << "10. Guardar en CSV\n";
        std::cout << "11. Cargar desde CSV\n";
        std::cout << "0. Salir\n";
        std::cout << "Ingrese su opcion: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1: {
                hoja.agregarFila();
                std::cout << "Fila agregada correctamente.\n";
                break;
            }
            case 2: {
                size_t index = leerTamano("Ingrese el índice de la fila a eliminar: ");
                hoja.eliminarFila(index);
                std::cout << "Fila eliminada correctamente.\n";
                break;
            }
            case 3: {
                hoja.agregarColumna();
                std::cout << "Columna agregada correctamente.\n";
                break;
            }
            case 4: {
                size_t index = leerTamano("Ingrese el índice de la columna a eliminar: ");
                hoja.eliminarColumna(index);
                std::cout << "Columna eliminada correctamente.\n";
                break;
            }
            case 5: {
                size_t fila = leerTamano("Ingrese el índice de la fila: ");
                size_t columna = leerTamano("Ingrese el índice de la columna: ");
                double valor = leerNumero();
                hoja.actualizarCelda(fila, columna, valor);
                std::cout << "Celda actualizada correctamente.\n";
                break;
            }
            case 6: {
                size_t fila = leerTamano("Ingrese el índice de la fila: ");
                size_t columna = leerTamano("Ingrese el índice de la columna: ");
                double valor = leerNumero();
                hoja.actualizarCelda(fila, columna, valor);
                std::cout << "Valor agregado correctamente.\n";
                break;
            }
            case 7: {
                size_t fila1 = leerTamano("Ingrese el índice de la fila 1: ");
                size_t col1 = leerTamano("Ingrese el índice de la columna 1: ");
                size_t fila2 = leerTamano("Ingrese el índice de la fila 2: ");
                size_t col2 = leerTamano("Ingrese el índice de la columna 2: ");
                char operacion;
                std::cout << "Ingrese la operacion (+, -, *, /): ";
                std::cin >> operacion;
                double resultado = hoja.operarCeldas(fila1, col1, fila2, col2, operacion);
                std::cout << "Resultado: " << resultado << std::endl;
                break;
            }
            case 8: {
                size_t fila = leerTamano("Ingrese el índice de la fila: ");
                char operacion;
                std::cout << "Ingrese la operacion (+, -, *, /): ";
                std::cin >> operacion;
                double resultado = hoja.operarFila(fila, operacion);
                std::cout << "Resultado: " << resultado << std::endl;
                break;
            }
            case 9: {
                size_t columna = leerTamano("Ingrese el índice de la columna: ");
                char operacion;
                std::cout << "Ingrese la operacion (+, -, *, /): ";
                std::cin >> operacion;
                double resultado = hoja.operarColumna(columna, operacion);
                std::cout << "Resultado: " << resultado << std::endl;
                break;
            }
            case 10: {
                std::string nombreArchivo;
                std::cout << "Ingrese el nombre del archivo CSV para guardar: ";
                std::cin >> nombreArchivo;
                hoja.guardarCSV(nombreArchivo);
                std::cout << "Datos guardados correctamente en " << nombreArchivo << ".\n";
                break;
            }
            case 11: {
                std::string nombreArchivo;
                std::cout << "Ingrese el nombre del archivo CSV para cargar: ";
                std::cin >> nombreArchivo;
                hoja.cargarCSV(nombreArchivo);
                hoja.mostrar(); // Mostrar la hoja después de cargar
                break;
            }
            case 0:
                std::cout << "Saliendo del programa...\n";
                break;
            default:
                std::cout << "Opción inválida, por favor intente de nuevo.\n";
                break;
        }

        std::cout << "Presione Enter para continuar...";
        std::cin.ignore();
        std::cin.get();

    } while (opcion != 0);
}

int main() {
    HojaCalculo hoja;
    menu(hoja);
    return 0;
}
