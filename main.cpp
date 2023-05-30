#include <iostream>
#include <fstream>
#include <chrono>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>

std::map<std::string, std::vector<int>> pasikartojantys_zodziai;
std::map<std::string, std::vector<int>> pasikartojantys_URL;

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

std::string tikrina_zodi(std::string zodis) {
    if (zodis.find("www.") != std::string::npos || 
        zodis.find("http://") != std::string::npos || 
        zodis.find("https://") != std::string::npos ||
        zodis.find(".lt") != std::string::npos ||
        zodis.find(".com") != std::string::npos ||
        zodis.find(".eu") != std::string::npos) {
        // Žodis atrodo kaip URL, tad paliekamas
        return zodis;
    }
     else {
        // Tikriname ar turi skaičių, nepageidaujamų simbolių
        const std::string nenorimi_simboliai = "„“–";
        for (char& c : zodis) {
            if (std::ispunct(c) || std::isdigit(c) || nenorimi_simboliai.find(c) != std::string::npos) {
                c = ' ';
            }
        }
        zodis.erase(std::remove_if(zodis.begin(), zodis.end(), ::isspace), zodis.end());
        return zodis;
    }
}

// Grąžina true, jeigu žodis yra URL
bool yra_URL(std::string zodis) {
    return zodis.find("www.") != std::string::npos || 
           zodis.find("http://") != std::string::npos || 
           zodis.find("https://") != std::string::npos ||
           zodis.find(".lt") != std::string::npos ||
           zodis.find(".com") != std::string::npos ||
           zodis.find(".eu") != std::string::npos;
}

void failo_nuskaitymas(std::string failo_pavadinimas){

    std::ifstream skaitomas_failas;
    skaitomas_failas.open(failo_pavadinimas);

    std::string nuskaityta_eilute;

    // rodys, kuriose eilutėse buvo rastas žodis
    int eilutes_numeris = 1;

    // Skaitoma atidaryto failo kiekviena eilutė
    while (std::getline(skaitomas_failas, nuskaityta_eilute)) {
        std::istringstream eilute(nuskaityta_eilute);
        std::string zodis;

        while (eilute >> zodis) {
            zodis = tikrina_zodi(zodis);

            // Keičiame į mažasias, kad išvedamame faile nesimaišytų su didžiosiomis raidėmis
            std::transform(zodis.begin(), zodis.end(), zodis.begin(), ::tolower);

            // prieš pridedant žodį į map, tikriname ar jis tuščias
            if (!zodis.empty()) {

                // tikrinam ar žodis yra URL, kad žinotume kur dėti
                if(yra_URL(zodis) == false){
                    if (pasikartojantys_zodziai.count(zodis) > 0) {
                        pasikartojantys_zodziai[zodis].push_back(eilutes_numeris);
                    }
                    else { 
                        pasikartojantys_zodziai[zodis];
                        pasikartojantys_zodziai[zodis].push_back(eilutes_numeris); 

                    }
                }
                // žodis nėra URL
                else{
                    if (pasikartojantys_URL.count(zodis) > 0) { 
                        pasikartojantys_URL[zodis].push_back(eilutes_numeris);
                    }
                    else {
                        pasikartojantys_URL[zodis];
                        pasikartojantys_URL[zodis].push_back(eilutes_numeris);
                    }
                }
            }
        }
        eilutes_numeris++;
    }
    // ištriname tuos žodžius, kurie pasikartojo tik kartą
    for(auto it = pasikartojantys_zodziai.cbegin(); it != pasikartojantys_zodziai.cend(); ) {
        if(it->second.size() < 2) {
            it = pasikartojantys_zodziai.erase(it); 
        }
        else {
            ++it;
        }
    }
    skaitomas_failas.close();
};

void lenteles_kurimas(std::map<std::string, std::vector<int>> duomenys_zodziai, std::map<std::string, std::vector<int>> duomenys_URL){

    std::ostringstream buferis;

    buferis << "Žodis                                   Pasikartojimų skaičius\n";
    buferis << "-------------------------------------------------------------------\n";

    for (const auto& zodis : duomenys_zodziai) {
        buferis << std::setw(40) << std::left << zodis.first;
        buferis << zodis.second.size() << '\n';
    }

    buferis << "-------------------------------------------------------------------\n";

    for (const auto& zodis : duomenys_URL) {
        buferis << std::setw(40) << std::left << zodis.first;
        buferis << zodis.second.size() << '\n';
    }

    const std::string& isvedimo_string = buferis.str();

    std::ofstream lenteles_failas("zodziuSkaicius.txt", std::ios_base::binary);
    lenteles_failas.write(isvedimo_string.c_str(), isvedimo_string.size());
    lenteles_failas.close();
}

void reference_kurimas(std::map<std::string, std::vector<int>> duomenys_zodziai, std::map<std::string, std::vector<int>> duomenys_URL){

    std::ostringstream buferis;

    buferis << "Žodis                                          Eilutės, kuriose randamas žodis\n";
    buferis << "-----------------------------------------------------------------------------\n";

    for (const auto& zodis : duomenys_zodziai) {
        buferis << std::setw(50) << std::left << zodis.first;
        buferis << "| ";
        for(int i = 0; i < zodis.second.size(); i++){
            buferis << zodis.second[i] << " | ";
        }
        buferis << std::endl;
    }

    buferis << "-----------------------------------------------------------------------------\n";

    for (const auto& zodis : duomenys_URL) {
        buferis << std::setw(50) << std::left << zodis.first;
        buferis << "| ";
        for(int i = 0; i < zodis.second.size(); i++){
            buferis << zodis.second[i] << " | ";
        }
        buferis << std::endl;
    }

    const std::string& isvedimo_string = buferis.str();

    std::ofstream lenteles_failas("referenceLentele.txt", std::ios_base::binary);
    lenteles_failas.write(isvedimo_string.c_str(), isvedimo_string.size());
    lenteles_failas.close();
}

void failo_tikrinimas(){

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
        std::cout << "Pabandykite dar kartą\n";
        std::cout << "Failo pavadinimas:   ";
        std::cin >> ivestis;
        std::cout << std::endl;
        failo_pavadinimas = "./" + ivestis;
    }
}
    // Kviečiame funkciją, kuri nuskaitys šį failą ir pakoreguos mūsų du std::map kintamuosius
    failo_nuskaitymas(failo_pavadinimas);

    // Kviečiame funkcijas, kurios sukurs žodžių ir jų skaičių bei reference lenteles
    lenteles_kurimas(pasikartojantys_zodziai,pasikartojantys_URL);
    reference_kurimas(pasikartojantys_zodziai,pasikartojantys_URL);

    std::cout << "Failas sėkmingai nuskaitytas!\n\n";
};

int main(){

    std::string pasirinkimas = irasymo_pasirinkimas();
    if(pasirinkimas == "1"){
        failo_tikrinimas();
    }

    return 0;
}