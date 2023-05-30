#include <iostream>
#include <fstream>
#include <chrono>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>

std::map<std::string, int> pasikartojantys_zodziai;
std::map<std::string, int> pasikartojantys_URL;

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

std::string processWord(std::string word) {
    if (word.find("www.") != std::string::npos || 
        word.find("http://") != std::string::npos || 
        word.find("https://") != std::string::npos ||
        word.find(".lt") != std::string::npos ||
        word.find(".com") != std::string::npos ||
        word.find(".eu") != std::string::npos) {
        // This word looks like a URL, so don't remove any punctuation
        return word;
    }
     else {

        const std::string unwanted_chars = "„“–";
        for (char& c : word) {
            if (std::ispunct(c) || std::isdigit(c) || unwanted_chars.find(c) != std::string::npos) {
                c = ' '; // replace unwanted characters with space
            }
        }
        word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end()); // remove spaces
        return word;
    }
}

bool is_URL(std::string word) {
    if (word.find("www.") != std::string::npos || 
        word.find("http://") != std::string::npos || 
        word.find("https://") != std::string::npos ||
        word.find(".lt") != std::string::npos ||
        word.find(".com") != std::string::npos ||
        word.find(".eu") != std::string::npos) {
        // This word looks like a URL, so don't remove any punctuation
        return true;
    }
    else {
        return false;
    }
}

void failo_nuskaitymas(std::string failo_pavadinimas){

    std::ifstream skaitomas_failas;
    skaitomas_failas.open(failo_pavadinimas);

    std::string nuskaityta_eilute;

    // Skaitoma atidaryto failo kiekviena eilutė

    while (std::getline(skaitomas_failas, nuskaityta_eilute)) {
        std::istringstream iss(nuskaityta_eilute);
        std::string word;

        while (iss >> word) {
            word = processWord(word);
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            if (!word.empty()) { // check if the word is not empty after removing spaces

                if(is_URL(word) == false){
                    if (pasikartojantys_zodziai.count(word) > 0) { // If the word is already in the map
                        ++pasikartojantys_zodziai[word]; // increment its count
                    }
                    else { // If the word is not in the map
                        pasikartojantys_zodziai[word] = 1; // add it to the map with a count of 1
                    }
                }
                else{
                    if (pasikartojantys_URL.count(word) > 0) { // If the word is already in the map
                        ++pasikartojantys_URL[word]; // increment its count
                    }
                    else { // If the word is not in the map
                        pasikartojantys_URL[word] = 1; // add it to the map with a count of 1
                    }
                }
            }
        }
    }
        auto it = pasikartojantys_zodziai.begin();
        while (it != pasikartojantys_zodziai.end()) {
            if (it->second < 2) {
                it = pasikartojantys_zodziai.erase(it);
            } 
            else {
                ++it;
            }
        }
    skaitomas_failas.close();
};

void lenteles_kurimas(std::map<std::string, int> duomenys_zodziai, std::map<std::string, int> duomenys_URL){

    std::ostringstream buferis;

    buferis << "Žodis                                   Pasikartojimų skaičius\n";
    buferis << "-------------------------------------------------------------------\n";

    for (const auto& zodis : duomenys_zodziai) {
        buferis << std::setw(40) << std::left << zodis.first;
        buferis << zodis.second << '\n';
    }

    buferis << "-------------------------------------------------------------------\n";

    for (const auto& zodis : duomenys_URL) {
        buferis << std::setw(40) << std::left << zodis.first;
        buferis << zodis.second << '\n';
    }

    const std::string& isvedimo_string = buferis.str();

    std::ofstream lenteles_failas("textas.txt", std::ios_base::binary);
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

    auto start = std::chrono::high_resolution_clock::now(); auto st=start;
    std::cout << "\n---------------- Įrasomi duomenys.... ----------------\n\n";

    // Kviečiam funkciją, kuri nuskaitys šį failą

    failo_nuskaitymas(failo_pavadinimas);

    // Kviečiam funkciją, kuri sukurs lentelę su žodžiais ir kiek kartų pasikartojo
    lenteles_kurimas(pasikartojantys_zodziai,pasikartojantys_URL);

    // surašomi programos veikimo laikai
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end-start;
    std::cout << "\nVisos programos veikimo laikas: "<< diff.count() << " s\n\n";  
    std::cout << "--------------------------------\n";
    std::cout << "Failas sėkmingai nuskaitytas!\n\n";

};


int main(){

    std::string pasirinkimas = irasymo_pasirinkimas();
    if(pasirinkimas == "1"){
        failo_tikrinimas();
    }

    return 0;
}