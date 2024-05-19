#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm> 
#include <sstream>
#include "classes.cpp"
using namespace std;

class ISAMFile{
    public:
    template <typename T>
    long binaryIndex(pair<T, long> arr[], long n, T key){
        long l = 1;
        long r = n-1;

        while (l < r){
            long mid = l + (r-l)/2;
            if (arr[mid].first < key)
                l = mid + 1;
            else
                r = mid;
        }
        if (l == n) return 0;
        return l;
    }

    template <typename T>
    pair<long, long> searchData(long y, long n, T key){

        fstream data(dfile, ios::app | ios::binary);
        // leer dp en posicion x;
        DataPage dpy;
        data.seekg(2*sizeof(long)+sizeof(char)+ y*(1+sizeof(DataPage)), ios::beg); // datapages
        data.read((char*) dpy, sizeof(dpy));

        while (dpy.nextPage != -1) {
            for (int i = 0; i < n; i++){
                if (dp.arr[i].fdc_id == key) return make_pair(y, i);
            }
            y = dpy.nextPage;
            data.seekg(2*sizeof(long)+sizeof(char)+ dpy.nextPage*(1+sizeof(DataPage)), ios::beg); // datapages
            data.read((char*) dpy, sizeof(dpy));
        }
        return make_pair(-1, -1);
    }

    template <typename T>
    long findpage(T key){
        ifstream file(ifile, ios::app | ios::binary | fstream::out);
        IndexPage<T> ip; 
        cout<<"hola"<<endl;
        
        int beg=0;
        int final=ip.n-1;

        file.seekg(4+1, ios::beg);
        file.read(sizeof(IndexPage), ip);
        
        //hago mi binary
        while(beg<=final) {
            int i=0;
            int mid=(beg+final)/2;
            
            if(key==ip.keys[mid].first){
                file.close();
            }
            if(key<=ip.keys[mid].first){
                
                final=mid-1;
            }
            else if(key>=ip.keys[mid].first){
                beg=mid+1;
            }
        }
        return ip.pages[beg];
        
    }
    
    template <typename T>
    vector<Record> search(T key){
        //obtengo mi ip2
        cout<<"hola"<<endl;
        long p2=findpage(key); 
        //paso a mi segundo nivel
        file.seekg(p2*(sizeof(IndexPage)+1), ios::beg); 
        file.read(ip,sizeof(IndexPage))

        //aplico binary de mi 2ndo nivel
        while(beg<=final) {
            int i=0;
            int mid=(beg+final)/2;
            
            if(key==ip.keys[mid]){
                file.close();
            }
            if(key<=ip.keys[mid]){
                cout<<"hola";
                final=mid-1;
            }
            else if(key>=ip.keys[i]){
                beg=mid+1;
            }
        }
        long ip3=indexPage.pages[beg]; 

        //tengo q acceder a datafile de mi p2
        ifstream file(dfile, ios::app | ios::binary | fstream::out);
        
        DataPage dataPage;
        file.seekg((4+1)+(1+sizeof(DataPage))*ip3, ios::beg);
        int beg=0;
        int final=file.n-1;

        
        //busco mi key en mi dp
        int i=0;
        while(dataPage.records[i]!=key){
            i++; 
        }

    }

    
    template <typename T>
    void add(Record r,T k){
        fstream file(ifile, ios::app | ios::binary | fstream::out);
        IndexPage<T> ip0;  //mi primera page
        
        int beg=0;
        int final=ip0.n-1;

        file.seekg(4+1, ios::beg); //para ubicar la ip0
        if(ip0.n!=ip0.MI-1){ //no esta lleno
            IndexPage<T> ipn;  //mi next ip
            ip0.pages[-1]=ipn; //mi ultimo page apunta al next level page
            //sort();
            //me ubico en ipn y escribo el record
            file.seekg(ipn*(sizeof(IndexPage)+1), ios::beg); 
            file.write(reinterpret_cast<const char*>(&k), sizeof(Record));

            //creo mi datapage para añadir mi record
            ifstream dfile(dfile, ios::app | ios::binary | fstream::out);
            DataPage dpn;
            dfile.seekg((4+1)+(1+sizeof(DataPage))*ipn, ios::beg);
            dfile.write(reinterpret_cast<const char*>(&k), sizeof(Record));

        }
        else{
            file.seekg(4+1, ios::beg);
            file.read(ip0,sizeof(IndexPage));

            //hago mi binary para hallar el mayor key a k y obtener pos
            while(beg<=final) {
                int i=0;
                int mid=(beg+final)/2;
                
                if(k==ip0.keys[mid]){
                    return mid;
                    file.close();
                }
                if(k<=ip0.keys[mid]){
                    final=mid-1;
                }
                else if(k>=ip0.keys[mid]){
                    beg=mid+1;
                }
            }
            long x=ip0.pages[beg]; //hallo la pos para mi segundo nivel 
            //ipx a memoria
            file.seekg((4+1)+(x*(sizeof(IndexPage)+1)), ios::beg); 
            IndexPage <T> ipx;
            if(ipx.n!=ipx.MI-1){
                DataPage dpn; //creo mi datapage
                ipx.pages[x]=dpn;
            }

            else{
                fstream dfile(dfile, ios::app | ios::binary | fstream::out);
                file.seekg((4+1)+(1+sizeof(DataPage))*ipx, ios::beg);
                DataPage dp; 
                
                int beg=0;
                int final=dp.n;
                //binary search para hallar el primer mayor a k y obtener pos de page
                while(beg<=final) {
                    int i=0;
                    int mid=(beg+final)/2;
                    
                    if(k==dp.records[mid]){
                        return mid;
                        dfile.close();
                    }
                    if(k<=dp.record[mid]){
                        final=mid-1;
                    }
                    else if(k>=dp.record[mid]){
                        beg=mid+1;
                    }
                }
                //me ubico en la pagina de mi dp
                long dpn=dp.record[beg]; 

                if(dp.n!=dp.MD-1){ //si no esta full
                    int pos=dp.nextdel; //uso free list para ver el next del y ubicarlo
                    dfile.seekg((4+1)+(1+sizeof(DataPage))*pos, ios::beg);
                    dfile.write(reinterpret_cast<const char*>(&k), sizeof(Record));  
                }
                else{
                    DataPage dpe; 
                    DataPage lastelem=dpn.records[-1]; //ultimo elemento
                    lastelem.nextPage=dpe; //su nextpage será el dpe
                    dpe.write(reinterpret_cast<const char*>(&k), sizeof(Record));  //escribo record
                }
            }
        }
    };

    template <typename T>
    bool remove(T key){
        fstream indexes(ifile, ios::binary |  ios::app)
        indexes.seekg(2*sizeof(int)+sizeof(char), ios::beg);

        IndexPage ip0;
        indexes.read((char*)ip0, sizeof(ip0)); // leo el primer index page

        long i = binaryIndex(ip0.keys, ip0.n, key); // busqueda en nivel 2
        long x = ip0.keys.second[i];
        
        IndexPage ipx;
        indexes.seekg(2*sizeof(long)+sizeof(char)+ x*(sizeof(IndexPage)+1), ios::beg); // segundo nivel de index pages
        indexes.read((char*)ipx, sizeof(ipx));

        long j = binaryIndex(ipx.keys, ipx.n, key); // busqueda para hallar datapage con el key
        long y = ipy.keys.second[j];

        fstream data(dpage, ios::binary | ios::app);
        
        
        pair<long,long> ele = searchData(y, ele);

        DataPage dpy;
        data.seekg(2*sizeof(long)+sizeof(char)+ ele.first*(1+sizeof(DataPage)), ios::beg); // datapages
        data.read((char*) dpy, sizeof(dpy));

        if (ele != -1){
            if (dpy.n == 1){
                dpy.nd = -1;
                long dataND; // nextdel de la cabecera del archivo de datapages
                data.seekg(sizeof(long), ios::beg);
                data.read(reinterpret_cast<const char*>(&dataND), sizeof(long));
                dataND = ele.first; // cambiamos el nextdel de la cabecera del archivo de data
                data.seekp(sizeof(long), ios::beg);
                data.write(reinterpret_cast<char*>(&dataND), sizeof(long));

                if (ipx.n == 1){
                    long indexND; // nextdel de la cabecera del archivo de indexes
                    indexes.seekg(sizeof(long), ios::beg);
                    indexes.read(reinterpret_cast<const char*>(&indexND), sizeof(long));
                    
                    if (indexND != -1) ipx.nextdel = indexND;
                    indexND = x;
                    indexes.seekp(sizeof(long), ios::beg);
                    indexes.write(reinterpret_cast<char*>(&indexND), sizeof(long)); // escribimos el nuevo nextdel en el archivo
                    ipx.n--;
                    
                    if (ip0.n == 1){ // si solo hay un indice en ip0
                        ip0.n = 0;
                        indexes.seekp(2*sizeof(long)+sizeof(char), ios::beg); // cambiar el tamaño de ip0 a 0
                        indexes.write((char*) &ip0, sizeof(ip0));
                    } else { // si no
                        ip0.keys[j] = ip0.keys[n]; // mover el ultimo key a la posicion del eliminado
                        ip0.n--;
                        sort(ip0.keys+1, ipx.keys+ipx.n-1); // sortear
                    }
                } else {
                    ipx.keys[j] = ipx.keys[n]; // mover el ultimo key a la posicion del eliminado
                    ipx.n--;
                    sort(ipx.keys+1, ipx.keys+ipx.n-1); // sortear
                }
                indexes.seekp(2*sizeof(long)+sizeof(char)+ x*(sizeof(IndexPage)+1), ios::beg); // segundo nivel de index pages
                indexes.write((char*)ipx, sizeof(ipx));
            } else {
                if (dpy.nd != -1) dpy.nextdel[ele.second] = dpy.nd; 
                dpy.nd = ele.second;
                data.seekp(2*sizeof(long)+sizeof(char)+ ele.first*(1+sizeof(DataPage)), ios::beg); // datapages
                data.write((char*) dpy, sizeof(dpy));
            }
        } else {
            cout << "record not found" << endl;
        }
        indexes.close();
        data.close();
    }

    
   template <typename T>
    vector <IndexPage<T>> rangesearchprev(T kinicio, T kfinal){
    //empiezo con mi ip0
    ifstream file(ifile, ios::app | ios::binary | fstream::out);
        IndexPage<T> ip; 
        vector <long> vindex_primernivel; 
        //en este vector guardaré los indexpages que estan en el rango
        
        int beg=0;
        int final=ip.n-1;

        file.seekg(2*sizeof(long)+sizeof(char), ios::beg);
        file.read(sizeof(IndexPage), ip);

        //hago mi binary
        while(beg<=final) {
            int i=0;
            int mid=(beg+final)/2;
            
            //if(ip.keys[mid]==kinicio){
            //    return mid;
            //    file.close();
            //}
            //not sure de estas condiciones
            if(ip.keys[mid].first>=kinicio && ip.keys[mid].first<=kfinal){
                vindex_primernivel.push_back(ip.keys[mid].first);
            }
            else if(ip.keys[mid].first>=kinicio && ip.keys[mid].first>=kfinal){
                final=mid-1;
            }
            else if(ip.keys[mid].first<=kinicio && ip.keys[mid].first<=kfinal){ 
                beg=mid+1;
            }
        }
        return vindex_primernivel;
   };

    template <typename T>
    bool loadAdd(Record r){
        T key = r.fdc_id;

        fstream indexes(ifile, ios::app | ios::binary);
        fstream data(dfile, ios::app | ios::binary);

        // traer ip0 a memoria ram
        IndexPage ip0;
        indexes.seekg(2*sizeof(int)+sizeof(char), ios::beg);
        indexes.read((char*)ip0, sizeof(ip0));
        if (ip0.n == MI){
            IndexPage ipx;
            long x = ip0.keys[MI].second;
            indexes.seekg(2*sizeof(long)+sizeof(char)+ x*(sizeof(IndexPage)+1), ios::beg); // segundo nivel de index pages
            indexes.read((char*)ipx, sizeof(ipx));
            if (ipx.n == MI){
                long y = ipx.keys[MI].second;
                DataPage dpy;
                data.seekg(2*sizeof(long)+sizeof(char)+ y*(sizeof(DataPage)+1), ios::beg); // segundo nivel de index pages
                data.read((char*)dpy, sizeof(dpy));
                if (dpy.n == MD) {
                    add(r, r.fdc_id);
                    indexes.close();
                    data.close();
                    return true;
                } else {
                    dpy.n++;
                    dpy.records[dpy.n] = r;
                    
                    // write dpy
                    data.seekp(2*sizeof(long)+sizeof(char)+ y*(1+sizeof(DataPage)), ios::beg); // datapages
                    data.write((char*) dpy, sizeof(dpy));
                } 
            } else {
                long datasz;
                data.seekg(sizeof(long), ios::beg);
                data.read(reinterpret_cast<const char*>(&datasz), sizeof(long));
                datasz++;

                data.seekp(sizeof(long), ios::beg);
                data.write(reinterpret_cast<char*> (&datasz), sizeof(long));
                
                ipx.n++;
                ipx.keys[ipx.n] = {r.fdc_id, datasz};

                // crear datapage

                DataPage dpy;
                dpy.records[0] = r;
                dpy.n++;

                data.seekg(2*sizeof(long)+sizeof(char)+ datasz*(sizeof(DataPage)+1), ios::beg);
                data.write((char*) &dpy, sizeof(dpy));
                data.write((char*) &nl, sizeof(char));
                
            }
        } else if (ip0.n == 0){
            IndexPage ipx;
            ip0.n++;
            ip0.keys = {r.fdc_id, 1};

            // crear new index
            ipx.keys = {r.fdc_id, 1};
            ipx.n++;

            // crear new datapage
            DataPage dpy;
            dpy.records[0] = r;
            dpy.n++;

            // write dpy
            data.seekp(2*sizeof(long)+sizeof(char), ios::beg); // datapages
            data.write((char*) &dpy, sizeof(dpy));
            indexes.write((char*) &nl, sizeof(char));

            // write ipx
            indexes.seekp(2*sizeof(long)+sizeof(char), ios::beg); // segundo nivel de index pages
            indexes.write((char*)&ipx, sizeof(ipx));
            indexes.write((char*) &nl, sizeof(char));

            // write ip0
            indexes.seekp(2*sizeof(int)+sizeof(char), ios::beg);
            indexes.write((char*)&ip0, sizeof(ip0));
            indexes.write((char*) &nl, sizeof(char));

            indexes.close();
            data.close();

            return true;
        }
        for (int i = 1; i < ip0.MI; i++){
            long x = ip0.keys[i].second;
            IndexPage ipx;
            indexes.seekg(2*sizeof(long)+sizeof(char)+ x*(sizeof(IndexPage)+1), ios::beg); // segundo nivel de index pages
            indexes.read((char*)ipx, sizeof(ipx));
            for (int j = 1; j < ipx.MI; j++){
                DataPage dpy;
                long y = ipx.keys[j].second;
                data.seekg(2*sizeof(long)+sizeof(char)+ y*(sizeof(DataPage)+1), ios::beg);
                data.read((char*) &dpy, sizeof(dpy));
                if (dpy.n == dpy.MD) continue;
                else {
                    dpy.n++;
                    dpy.records[dpy.n] = r;
                    data.seekp(2*sizeof(long)+sizeof(char)+ y*(sizeof(DataPage)+1), ios::beg);
                    data.write((char*) &dpy, sizeof(dpy));
                    indexes.close();
                    data.close();
                    return true;
                }
            }
        }

        // revisar si hay espacio en el primer index (nivel 2)
            // revisar si hay espacio en el primer datapage (nivel 3)
            // si no, ir al siguiente datapage. si no existe, crearlo
        // si no, crear e ir al siguiente index. si no existe, crearlo
        indexes.close();
        data.close();
        return false;
    }

    void loadData(const string& csvFilename) {
        ifstream file(csvFilename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << csvFilename << endl;
            return;
        }

        string line;
        getline(file, line);

        while (getline(file, line)) {
            stringstream ss(line);
            string token;

            Record record;

            // Leer fdc_id
            if (!getline(ss, token, ';')) {
                cerr << "Error reading fdc_id" << endl;
                continue;
            }
            try {
                record.fdc_id = stoi(token);
            } catch (const invalid_argument& e) {
                cerr << "Invalid fdc_id value: " << token << endl;
                continue;
            }

            // Leer brand
            if (!getline(ss, token, ';')) {
                cerr << "Error reading brand" << endl;
                continue;
            }
            strncpy(record.brand, token.c_str(), sizeof(record.brand) - 1);
            record.brand[sizeof(record.brand) - 1] = '\0';  // Asegurarse de que termine en nulo

            // Leer description
            if (!getline(ss, token, ';')) {
                cerr << "Error reading description" << endl;
                continue;
            }
            strncpy(record.description, token.c_str(), sizeof(record.description) - 1);
            record.description[sizeof(record.description) - 1] = '\0';  // Asegurarse de que termine en nulo

            // Leer ingredients
            if (!getline(ss, token, ';')) {
                cerr << "Error reading ingredients" << endl;
                continue;
            }
            strncpy(record.ingredients, token.c_str(), sizeof(record.ingredients) - 1);
            record.ingredients[sizeof(record.ingredients) - 1] = '\0';  // Asegurarse de que termine en nulo

            // Leer servingsize
            if (!getline(ss, token, ';')) {
                cerr << "Error reading servingsize" << endl;
                continue;
            }
            try {
                record.servingsize = stof(token);
            } catch (const invalid_argument& e) {
                cerr << "Invalid servingsize value: " << token << endl;
                continue;
            }

            // Insertar el registro en ISAM
            loadAdd<int>(record);
        }
    }


    template <typename T>
   vector<Record> rangesearch( T kinicio, T kfinal){
        vector <Record> vfinal;
        vector <IndexPage<T>> vindex_primernivel= rangesearchprev(kinicio, kfinal);// voy al next ip
        for(int i: vindex_primernivel){ //aca guardo los keys de primer nivel
            for (int element: vindex_primernivel[i]){ //aca itero sobre los elementos del primer nivel (segundo nivel)
                if(element=0){ //si es el primer elemento tengo que chequear desde dónde agregar
                    //entro a su dp
                    fstream dfile(dfile, ios::app | ios::binary | fstream::out);
                    //not sure de como seek aca
                    file.seekg((4+1)+(1+sizeof(DataPage))*vindex_primernivel[i][element], ios::beg);
                    DataPage dp; 

                    int beg=0;
                    int final= dp.n-1;
                    //lineal porque dp esta desordenado
                    if(vindex_primernivel[i][element]<=kfinal && vindex_primernivel[i][element]>=kinicio){
                        vfinal.push_back(dp.records[j])
                        j++;
                    }
                
                }
                //copio todos los demás elementos que estan between
                else if(element!=vindex_primernivel[i].n){ //not sure 
                    //entro a su dp
                    fstream dfile(dfile, ios::app | ios::binary | fstream::out);
                    //not sure de como seek aca
                    file.seekg((4+1)+(1+sizeof(DataPage))*vindex_primernivel[i][element], ios::beg);
                    DataPage dp; 
                    int j=0
                    int final=dp.n-1;
                    while(j<=final){
                        vfinal.push_back(dp.records[j])
                    }
                }
                else if (element+1>=kfinal){ //si ya el siguiente del 2ndo nivel supera mi kfinal
                    //veo los elementos del dp del element
                    fstream dfile(dfile, ios::app | ios::binary | fstream::out);
                    DataPage dp; 
                    //not sure de como seek aca
                    file.seekg((4+1)+(1+sizeof(DataPage))*vindex_primernivel[i][element], ios::beg);
                    int j=0;
                    while(vindex_primernivel[i][element]<=kfinal){
                        vfinal.push_back(dp.records[j])
                        j++;
                    }
                }
            }
        }
        
   };
};

/*
Busqueda(key):
- Abrir el index1 
- Leer la entrada relacionada a key (pagina del siguiente index)
- Abrir el index2
- Leer la entrada relacionada a key (pagina del siguiente index)
- Abrir el index3
- Leer la entrada en el archivo de datos(pagina del archivo de datos)
- Leer la pagina de datos y ubicas el registro con la key
- Buscar en el encadenamiento de paginas
:: O(L+1) + O(encadenamiento),  L total de niveles

Insertar(record, key):
- Abrir el index1 y localizamos la entrada  *
- Abrir el index2 y localizamos la entrada  *
- Abrir el index3 y localizamos la entrada  *
- Leer la pagina de datos se lleva a la RAM *
- Caso 1: pagina no llena
  - Insertar el registro en dicha pagina
  - Escribir la pagina en la misma posicion *
- Caso 2: pagina si esta llena
  - Crear nueva pagina de datos
  - Agregar el nuevo registro a dicha pagina (o aplicar split)
  - La pagina actual se enlaza con la pagina recien creada
  - Escribir pagina actual y pagina enlazada *
O(L) + O(encadenamiento)
*/

