#include "pci_.h"
#include <iostream>
#include <stdio.h>
#include <sys/io.h>
#include <string>
#include <bitset>
#include <bits/stdc++.h>
#include <fstream>
#define PCI_VENTABLE_LEN (sizeof(PciVenTable)/sizeof(PCI_VENTABLE))
#define PCI_DEVTABLE_LEN (sizeof(PciDevTable)/sizeof(PCI_DEVTABLE))

using namespace std;

int get_dev_id (long Addr);
int get_ven_id (long Addr);
void print_vendor (int venID);
void print_device (int devID, int venID);
bool is_bridge (long Addr);
int get_cache_line_size (long Addr);
void print_bus_numbers(long Addr);
void print_memory(long Addr);
void print_addr(int busid, int devid, int func);

int main() {
    iopl(3);
    
    std::ofstream out ("out.txt");
    
    long Addr=0, Res = 0;
    int busid, devid, func, reg=0; 
	for (busid  = 0; busid < 256; busid ++) {
		for (devid = 0; devid < 32; devid ++) {
			for (func = 0; func < 8; func++) {
                                   Addr = 2147483648 + (busid << 16) + (devid << 11) + (func << 8) + reg;
                                   outl(Addr, 0xCF8);
                                   Res = inl (0xCFC);
                                   	if (Res != 4294967295) {
                                   		print_addr(busid, devid, func);
                                   		int deviceID = get_dev_id (Res);
                                   		int vendorID = get_ven_id (Res);
                                   		print_vendor(vendorID);
                                   		print_device(deviceID, vendorID);
                                   			if (!is_bridge(Addr)) {
                                   				printf("Cache line size: %d Kb\n", get_cache_line_size(Addr));
                                                                              }
                                                        else {
                                                        	print_bus_numbers(Addr);
                                                        	print_memory(Addr);
                                                        }
                                                                cout << "************************" << endl;
                                  
   }
  }
 }
}
                                   
    return 0;
}

int get_dev_id (long Addr) {
    string x = std::bitset<32>(Addr).to_string();
    string fst = x.substr(0, 16);
    return stoi(fst, 0, 2);
 }

 int get_ven_id (long Addr) {
    string x = std::bitset<32>(Addr).to_string();
    string sec = x.substr(16);
    return stoi(sec, 0, 2);
 }
 
 void print_vendor (int venID) {
 	for (int i = 0; i < PCI_VENTABLE_LEN; i++) {
 		if (venID == PciVenTable[i].VendorId) {
 			printf ("Vendor: %s\n", PciVenTable[i].VendorName);
 		}
 	}
 }
 
 void print_device (int devID, int venID) {
 	for (int i = 0; i < PCI_DEVTABLE_LEN; i++) {
 		if (devID == PciDevTable[i].DeviceId && venID == PciDevTable[i].VendorId) {
 			printf ("Device: %s\n", PciDevTable[i].DeviceName);
 		}
 	}
 }
 
 bool is_bridge (long Addr) {
 	long newAddr = Addr + 12;
 	outl(newAddr, 0xCF8);
        long Res = inl (0xCFC);
        string x = std::bitset<32>(Res).to_string();
        string header_type = x.substr(8, 8);
        if (header_type[7] == '0') return false;
        return true;
 }
 
 int get_cache_line_size(long Addr) {
 	long newAddr = Addr + 12;
 	outl(newAddr, 0xCF8);
        long Res = inl (0xCFC);
        string x = std::bitset<32>(Res).to_string();
        string cache_line_size = x.substr(16);
        return stoi(cache_line_size, 0, 2);
 }
 
 void print_bus_numbers(long Addr) {
 	long newAddr = Addr + 24;
 	outl(newAddr, 0xCF8);
        long Res = inl (0xCFC);
        string x = std::bitset<32>(Res).to_string();
        string sub = x.substr(8, 8);
        string sec = x.substr(16, 8);
        string prim = x.substr(24, 8);
        
        printf("Subordinate Bus Number: %d\n", stoi(sub, 0, 2));
        printf("Secondary Bus Number: %d\n", stoi(sec, 0, 2));
        printf("Primary Bus Number: %d\n", stoi(prim, 0, 2));
 }
 
 void print_memory(long Addr) {
        long newAddr = Addr + 32;
 	outl(newAddr, 0xCF8);
        long Res = inl (0xCFC);
        string x = std::bitset<32>(Res).to_string();
        string mem_lim = x.substr(0, 16);
        string mem_base = x.substr(16);
        
        printf("Memory limit: %d\n", stoi(mem_lim, 0, 2));
        printf("Memory base: %d\n", stoi(mem_base, 0, 2));
 }
 
 void print_addr(int busid, int devid, int func) {	
 	printf("%x:%x:%x\n", busid, devid,func);
 }
 
 
 



