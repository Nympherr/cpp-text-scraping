#include <iostream>
#include <fstream>

std::string irasymo_pasirinkimas(){

    std::cout << "\n----------------------------------\n";
    std::cout << "         PROGRAMOS PRADŽIA\n";
    std::cout << "----------------------------------\n\n";

    std::cout << "Pasirinkite norimą operaciją:\n";
    std::cout << "1 - Failo nuskaitymas\n";
    std::cout << "2 - Išjungti programą\n";
    std::cout << "\nJūsų pasirinkimas:  ";

    std::string ivestis;
    std::cin >> ivestis;
    while(ivestis != "1" && ivestis != "2"){
        std::cout<< "\nGalite pasirinkti tik [1] arba [2]!\n";
        std::cout<< "Jūsų pasirinkimas:  ";
        std::cin >> ivestis;
        std::cout << std::endl;
    }

    return ivestis;
};

void failo_nuskaitymas(){

    // Duoda vartotojui pasirinkti failą, kurį jis nori atidaryti

    std::cout << "\nIrasykite failo pavadinima:   ";
    std::string ivestis;
    std::cin >> ivestis;

    std::string failo_pavadinimas = "./" + ivestis;
    std::ifstream skaitomas_failas;

    // Tikrina ar įvestis teisinga

   while (true) {
    try {
        skaitomas_failas.open(failo_pavadinimas);
        if (!skaitomas_failas.is_open()) {
            throw "\nNepavyko atidaryti failo";
        }
        break;
    }
    catch(const char* klaidos_zinute){
        std::cerr << klaidos_zinute << std::endl;
        std::cout << "Pabandykite dar karta\n";
        std::cout << "Failo pavadinimas:   ";
        std::cin >> ivestis;
        std::cout << std::endl;
        failo_pavadinimas = "./" + ivestis;
    }
}

    std::cout << "\n---------------- Irasomi duomenys.... ----------------\n\n";

    // Kviečiam funkciją, kuri nuskaitys šį failą ir studentus sudės į bendrą vektorių.

    std::cout << "Atlikta";

};


int main(){

    std::string pasirinkimas = irasymo_pasirinkimas();
    if(pasirinkimas == "1"){
        failo_nuskaitymas();
    }




    return 0;
}