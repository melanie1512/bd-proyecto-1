#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <bitset>
#include <map>
#include <cstring>

using namespace std;

const int D = 3; // Global depth
const int fb = 3; // Block factor
int lD;
// Función hash
template<typename T>
std::vector<char> fhash(T key) {
    std::hash<T> f_hash;
    auto hashed_key = f_hash(key);
    std::bitset<D> bit_sequence;
    for (std::size_t i = 0; i < D; ++i) {
        bit_sequence[i] = (hashed_key >> i) & 1;
    }

    std::vector<char> hash_vector;
    for (std::size_t i = 0; i < D; ++i) {
        hash_vector.push_back(bit_sequence[i] ? '1' : '0');
    }

    return hash_vector;
}

/*
funcion necesaria para manejo de hash indexes
std::vector<char> stringToVector(const std::string &str) {
    return std::vector<char>(str.begin(), str.end());
}
*/

template<typename T>
struct Record {
    T key;
    char nombre[12];
    int ciclo; // 4 bytes
    Record() {};
    Record(T key) {
        this->key = key;
    }
    Record(T key, const char* nom, int ciclo) : key(key), ciclo(ciclo) {
        strncpy(nombre, nom, sizeof(nombre) - 1);
        nombre[sizeof(nombre) - 1] = '\0'; // Asegurar que esté terminado en null
    }

     void print() const {
        cout << "Key: " << key << endl;
        cout << "Nombre: " << nombre << endl;
        cout << "Ciclo: " << ciclo << endl;
    }

};

// Overloads for writing Bucket and AdressRecord objects
struct AdressRecord {
    vector<char> hash_index; // Max size: D
    int bucket_id = 0;
    int d = 0;

    AdressRecord() {
        for (int i = 0; i < D; i++) {
            hash_index.push_back('-');
        }
    };

    AdressRecord(int bucket_id, char hashindex_) {
        this->d++;
        for (int i = 0; i < D; i++) {
            hash_index.push_back('-');
        }
        this->bucket_id = bucket_id;
        this->hash_index[D - 1] = hashindex_;
    }

    AdressRecord(int bucket_id, vector<char> vcopy) {
        this->bucket_id = bucket_id;
        this->hash_index = vcopy;
    }

    AdressRecord splitAdress(int newbucket_id) {
        if (d + 1 <= D) {
            hash_index[lD-D] ='0';
            }
        AdressRecord nuevoAR(newbucket_id, this->hash_index);
        nuevoAR.hash_index[lD-D] = '1';
        return nuevoAR;
    }

    void showAR() {
        cout << endl;
        for (int i = 0; i < D; i++) {
            cout << hash_index[i];
        }
        cout << " -> " << setw(3) << this->bucket_id << endl;
    }
};

bool same_hindex(vector<char> v1, vector<char> v2) {
    if (v1.size() != v2.size()) return false;
    for (int i = D-lD; i < D; i++) {
        if (v1[i] != v2[i]) return false;
    }
    return true;
}

int get_bucket_id(vector<char> hindex, string adressT) {
    int id = -1; // Default to -1 if not found
    ifstream adressTFile(adressT, ios::binary);
    if (!adressTFile) {
        cerr << "No se pudo abrir el archivo " << adressT << " para lectura." << endl;
        return -1;
    }

    int cant_arecords;
    adressTFile.seekg(0);
    adressTFile.read((char*)&cant_arecords, sizeof(int));

    AdressRecord temp;
    for (int i = 0; i < cant_arecords; ++i) {
        temp.hash_index.clear();
        for (int j = 0; j < D; ++j) {
            char ch;
            adressTFile.read(&ch, sizeof(char));
            temp.hash_index.push_back(ch);
        }
        adressTFile.read((char*)&temp.bucket_id, sizeof(int));
        if (same_hindex(temp.hash_index, hindex)) {
            adressTFile.close();
            id = temp.bucket_id;
            break; // Bucket ID found, break the loop
        }
    }

    adressTFile.close();
    return id;
}

template<typename T>
struct Bucket {
    T bucket_id;
    Record<T> records[fb];
    int next_bucket;
    int size;
    int local_d;

    Bucket() {
        local_d = 1;
        size = 0;
        next_bucket = -1;
        for (int i = 0; i < fb; ++i) {
            records[i] = Record<T>();
        }
        
    }

    Bucket(int bucket_id) {
        this->bucket_id = bucket_id;
        local_d = 1;
        size = 0;
        next_bucket = -1;
        for (int i = 0; i < fb; ++i) {
            records[i] = Record<T>();
        }
    }

    void showBK() {
        cout << "Datos Bucket : " << endl;
        cout << setw(3) << this->bucket_id;
        //como tiene size 0 los dos primeros buckets no se lee

        for (int i = 0; i < size; i++) {
             cout << setw(fb) << (this->records[i]).key;
        }

        cout << setw(3) << this->next_bucket;
        cout << setw(3) << this->size;
        cout << setw(3) << this->local_d << endl;
        
    }
};

template<typename T>
class ExtendibleHashing {
private:
    string hashfile;
    string adressT;
    int n_buckets;
    int n_overflow;

public:


 int split(vector<char> rhindex){
    ifstream iAdressT(adressT, ios::binary);
    iAdressT.seekg(0, ios::beg);
    int cant_b_hi = 0;
    iAdressT.read((char*)&cant_b_hi, sizeof(int));
    AdressRecord temp;
    for(int i=0;i < cant_b_hi;i++){
        for (int j = 0; j < D; j++) {
            iAdressT.read((char*)&temp.hash_index[j], sizeof(char));
        }
        iAdressT.read((char*)&temp.bucket_id, sizeof(int));
        if(same_hindex(temp.hash_index, rhindex)){
          //encontró el adressrecord a splitear
          break;

        }
    }
    iAdressT.close();
    //nos quedamos con temp
    cant_b_hi++;
    AdressRecord nuevo = temp.splitAdress(cant_b_hi);
    //escribir ese nuevo adressrecord
    ofstream oAdressT(adressT, ios::binary | ios::app);
    //al final
    for(char ch:nuevo.hash_index)
    {
      oAdressT.write(&ch, sizeof(char));
    }
    oAdressT.write(reinterpret_cast<char*>(&nuevo.bucket_id), sizeof(int));

    oAdressT.close();
    //spliteo hecho en adressT
    //Ahora en hashfile hay que reinsertar, retornamos el bucket id del segundo bucket split.
    
     return nuevo.bucket_id;
   }

    void showHashfile() {
        ifstream iHashfile(hashfile, ios::binary);
        iHashfile.seekg(0, ios::beg);

        Bucket<T> reading_bucket;
        for (int i = 0; i < n_buckets; ++i) {
          
            iHashfile.read(reinterpret_cast<char*>(&reading_bucket.bucket_id), sizeof(int));
            //iHashfile.read(reinterpret_cast<char*>(reading_bucket.records), fb * sizeof(Record<T>));
            iHashfile.seekg(i*(16+fb*sizeof(Record<T>))+4+fb*sizeof(Record<T>), ios::beg);
            iHashfile.read(reinterpret_cast<char*>(&reading_bucket.next_bucket), sizeof(int));
            iHashfile.read(reinterpret_cast<char*>(&reading_bucket.size), sizeof(int));
            iHashfile.read(reinterpret_cast<char*>(&reading_bucket.local_d), sizeof(int));
            
            if(reading_bucket.size>=1){
              //lea las keys
              for(int j=0;j<reading_bucket.size;j++)
              {
                 iHashfile.seekg(i*(16+fb*sizeof(Record<T>))+4+j*sizeof(Record<T>), ios::beg);
                 iHashfile.read((char*)&((reading_bucket.records[j]).key), sizeof((reading_bucket.records[j]).key));

              }

            }
            

            reading_bucket.showBK();
        }
        iHashfile.close();
    }

   void showAdressTable(){
    ifstream iAdressT(adressT, ios::binary);
    iAdressT.seekg(0, ios::beg);
    int cant_b_hi = 0;
    iAdressT.read((char*)&cant_b_hi, sizeof(int));
    AdressRecord temp;
    iAdressT.seekg(2*sizeof(int), ios::beg);
    for(int i=0;i < cant_b_hi;i++){
        for (int j = 0; j < D; j++) {
            iAdressT.read((char*)&temp.hash_index[j], sizeof(char));
        }
        iAdressT.read((char*)&temp.bucket_id, sizeof(int));
        temp.showAR();
    }


   }


    ExtendibleHashing(string hashfile, string adressT) {
        this->hashfile = hashfile;
        this->adressT = adressT;

        ifstream checkFile(adressT, ios::binary);
        bool fileExists = checkFile.is_open();
        checkFile.close();

        if (!fileExists) {
            ofstream adressTFile(adressT, ios::binary);
            if (!adressTFile) {
                cerr << "No se pudo crear el archivo " << adressT << endl;
                return;
            }

            this->n_buckets = 2;
            adressTFile.write(reinterpret_cast<char*>(&n_buckets), sizeof(int));
             

            //importante numero de buckets overflow
            this->n_overflow=0;
            adressTFile.write(reinterpret_cast<char*>(&n_overflow), sizeof(int));
            AdressRecord r1(0, '0');
            AdressRecord r2(1, '1');

            for (char ch : r1.hash_index) {
                adressTFile.write(&ch, sizeof(char));
            }
            adressTFile.write(reinterpret_cast<char*>(&r1.bucket_id), sizeof(int));

            for (char ch : r2.hash_index) {
                adressTFile.write(&ch, sizeof(char));
            }
            adressTFile.write(reinterpret_cast<char*>(&r2.bucket_id), sizeof(int));

            adressTFile.close();

            ofstream oHashfile(hashfile, ios::binary);
            if (!oHashfile) {
                cerr << "No se pudo crear el archivo " << hashfile << endl;
                return;
            }
            oHashfile.seekp(0, ios::beg);
            Bucket<T> initial_bucket;
            for (int i = 0; i < n_buckets; ++i) {
                initial_bucket.bucket_id = i;
                oHashfile.write(reinterpret_cast<char*>(&initial_bucket), sizeof(Bucket<T>));
            }
            oHashfile.close();
            
        } else {
          //leer cant buckets

            ifstream adressTFile(adressT, ios::binary);
            if (!adressTFile) {
                cerr << "No se pudo abrir el archivo " << adressT << " para lectura." << endl;
                return;
            }
               
            adressTFile.seekg(0, ios::beg);
            adressTFile.read(reinterpret_cast<char*>(&n_buckets), sizeof(int));
            adressTFile.read(reinterpret_cast<char*>(&n_overflow), sizeof(int));
            adressTFile.close();
            /*
            cout << "El achivo AdressTfile ya existia entonces leemos " << this->n_buckets << " como numero de buckets en el AdressTFile. " << endl;

            AdressRecord temp;
            for (int i = 0; i < n_buckets; i++) {
                for (int j = 0; j < D; j++) {
                    char ch;
                    adressTFile.read(&ch, sizeof(char));
                    temp.hash_index[j] = ch;
                }
                adressTFile.read(reinterpret_cast<char*>(&temp.bucket_id), sizeof(int));
                cout << "---------------------" << endl;
                for (char ch : temp.hash_index) {
                    cout << ch;
                }
                cout << " -> " << temp.bucket_id << endl;
            }

            

            cout << "Lectura de AdressTFile terminada. " << endl;
            */
        }
        cout<<"AdressTable: ";
        showAdressTable();
        cout<<"Hashfile: #buckets("<<this->n_buckets<<")"<<endl;
        showHashfile();
    }

    void insert_(T key)
    {
      vector<char> rhindex = fhash(key); // T
      int bkid = get_bucket_id(rhindex, adressT);

      // Abre el archivo para lectura y escritura
      fstream hashfile(this->hashfile, ios::binary | ios::in | ios::out);
      if (!hashfile) {
          cerr << "No se pudo abrir el archivo " << this->hashfile << " para lectura/escritura." << endl;
          return;
      }

      // Desplazarse a la posición del bucket
      size_t bucket_position = bkid * (16 + fb * sizeof(Record<T>));
      cout<<"bucket pos: "<<bkid<<endl;
      hashfile.seekg(bucket_position, ios::beg);

      // Leer el tamaño actual del bucket
      int bs = 0;
      hashfile.seekg(8 + fb * sizeof(Record<T>), ios::cur);
      hashfile.read(reinterpret_cast<char*>(&bs), sizeof(int));



      cout << "Size : " << bs << " hay espacio." << endl;
        
        // Incrementar el tamaño del bucket
        bs++;
        int actbs=bs;
        // Escribir el nuevo tamaño del bucket
        hashfile.seekp(bucket_position + 8 + fb * sizeof(Record<T>), ios::beg);
        hashfile.write(reinterpret_cast<char*>(&actbs), sizeof(int));
        
        
        //Escribir el nuevo record
        hashfile.seekp(bucket_position + 4 + (bs-1)*sizeof(Record<T>), ios::beg);
        hashfile.write(reinterpret_cast<char*>(&key), sizeof(T));
       
        
        cout << "Registro REinsertado correctamente." << endl;
    }
   void insert(Record<T> record) {
    //rhindex : record hash index
    vector<char> rhindex = fhash(record.key); // T
    int bkid = get_bucket_id(rhindex, adressT);

    // Abre el archivo para lectura y escritura
    fstream hashfile(this->hashfile, ios::binary | ios::in | ios::out);
    if (!hashfile) {
        cerr << "No se pudo abrir el archivo " << this->hashfile << " para lectura/escritura." << endl;
        return;
    }

    // Desplazarse a la posición del bucket
    size_t bucket_position = bkid * (16 + fb * sizeof(Record<T>));
    cout<<"bucket pos: "<<bkid<<endl;
    hashfile.seekg(bucket_position, ios::beg);

    // Leer el tamaño actual del bucket
    int bs = 0;
    hashfile.seekg(8 + fb * sizeof(Record<T>), ios::cur);
    hashfile.read(reinterpret_cast<char*>(&bs), sizeof(int));

    // Verificar si hay espacio en el bucket
    if (bs < fb) {
        cout << "Size : " << bs << " hay espacio." << endl;
        
        // Incrementar el tamaño del bucket
        bs++;
        int actbs=bs;
        // Escribir el nuevo tamaño del bucket
        hashfile.seekp(bucket_position + 8 + fb * sizeof(Record<T>), ios::beg);
        hashfile.write(reinterpret_cast<char*>(&actbs), sizeof(int));
        
        
        //Escribir el nuevo record
        hashfile.seekp(bucket_position + 4 + (bs-1)*sizeof(Record<T>), ios::beg);
        hashfile.write(reinterpret_cast<char*>(&record.key), sizeof(record.key));
       
        
        cout << "Registro insertado correctamente." << endl;
    } else {
        //bucket lleno, ver si se puede splitear
        if(lD < D){ //si se puede splitear sin encadenamiento
           //como se hara una reinsercion, extraer los keys de ese bucket para reinsertarlos
           vector<T> mykeys;
           T thekey;
           for(int i=0; i<fb; i++)
           {
            hashfile.seekg(bucket_position + 4 + (i*sizeof(Record<T>)), ios::beg);
            hashfile.read((char*)&thekey, sizeof(T));
            mykeys.push_back(thekey);
           }
           //sabemos que en la reinsercion no habra overflow, etonces solo insertamos en ambos buckets
           int bkid_n = split(rhindex);
           //le cambiamos el size,
            bs=0;
            hashfile.seekp(bucket_position + 8 + fb * sizeof(Record<T>), ios::beg);
            hashfile.write(reinterpret_cast<char*>(&bs), sizeof(int));
            for(T tkey:mykeys){
              insert_(key);
            }
        
         lD++;
         n_buckets++;
        }
        else{
        //procedemos al encadenamiento
        //crear nuevo bucket
        Bucket<T> overflowbucket;
        Bucket<T> mainbucket;
        mainbucket.next_bucket=900;
        //primero al bucket que le corresponde al hashindex del key, verificar que lleguemos al final overflow bucket
        hashfile.seekg(0, ios::beg);
        int k=0;
        while(mainbucket.next_bucket != -1)
        {
          hashfile.seekg(i*(16 +fb*sizeof(Record<T>)) + 4 + fb*sizeof(Record<T>) , ios::beg);
          hashfile.read((char*)&mainbucket.next_bucket, sizeof(int));
          k++;
        }
         //encontro el ultimo bucket para crearle su overflow
        n_overflow+; //diferencia entre buckets de adressT y hashfile
        overflowbucket.bucket_id = n_buckets + n_overflow;

        hashfile.seekp(k*(16 +fb*sizeof(Record<T>)) + 4 + fb*sizeof(Record<T>) , ios::beg);
        hashfile.write((char*)&overflowbucket.bucket_id, sizeof(int));
        //asigncacion
        overflowbucket.recods[overflowbucket.size] = record;
        //Escritura
        hashfile.seekp(0, ios::app);
        hashfile.write(reinterpret_cast<char*>(&overflowbucket.bucket_id), sizeof(int));
        //
        hashfile.seekp(fb*sizeof(Record<T>), ios::cur);
        hashfile.write(reinterpret_cast<char*>(&overflowbucket.next_bucket), sizeof(int));
        hashfile.write(reinterpret_cast<char*>(&overflowbucket.size), sizeof(int));
        hashfile.write(reinterpret_cast<char*>(&overflowbucket.local_d), sizeof(int));


        }
        
    }

    hashfile.close();
}

};

