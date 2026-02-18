#include<thread>
#include<string>
#include<iostream>
#include<vector>

struct datum {long long start,end; double sum;};
void harm(datum& arg){
    long long start = arg.start, end=arg.end;
    double sum = 0;
    for(long long i=start; i<end; i++) sum += 1.0/(double)i;
    arg.sum = sum;
}


int main(int argc, char** argv) {
    long long nterms = 1000000000000000; int nthreads=1;
    for(int i=0;i<argc;i++) {
        std::string arg = argv[i];
        if(arg=="-terms" && i+1 < argc) nterms=(int)std::stoll(argv[++i]);
        if(arg=="-threads" && i+1 < argc) nthreads=(int)std::stoi(argv[++i]);
    }
    std::cout << "terms: " << nterms << std::endl;
    std::cout << "threads: " << nthreads << std::endl;

    std::vector<std::thread> threads;
    threads.reserve(nthreads);
    std::vector<datum> data(nthreads);
    for(int i=0;i<nthreads;++i){
        data[i].start=1+(nterms/nthreads)*i;
        data[i].end=1+(nterms/nthreads)*(i+1);
        threads.emplace_back(harm,std::ref(data[i]));
    }
    for(std::thread &thread : threads)thread.join();
    double total =0;
    for(datum &d : data)total += d.sum;
    std::cout << "Total sum = " << total << std::endl;
    

return 0;}