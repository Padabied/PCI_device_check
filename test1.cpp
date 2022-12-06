#include "pci_.h"
#include <iostream>
#include <stdio.h>
#include <sys/io.h>
#include <string>
#include <bitset>
#include <bits/stdc++.h>
#define PCI_VENTABLE_LEN (sizeof(PciVenTable)/sizeof(PCI_VENTABLE))
#define PCI_DEVTABLE_LEN (sizeof(PciDevTable)/sizeof(PCI_DEVTABLE))

using namespace std;

int get_dev_id (long Addr);
int get_ven_id (long Addr);
void print_vendor (int venID);
void print_device (int devID, int venID);
bool is_bridge (long Addr);
int get_cache_line_size (long Addr);

struct registers {
    int devAndVenID = 0x00;
    int headerType = 0x0C;
};

int main() {
    iopl(3);
    
    long Addr=0, Res = 0;
    int busid, devid, func, reg=0; 
	for (busid  = 0; busid < 256; busid ++) {
		for (devid = 0; devid < 32; devid ++) {
			for (func = 0; func < 8; func++) {
                                   Addr = 2147483648 + (busid << 16) + (devid << 11) + (func << 8) + reg;
                                   outl(Addr, 0xCF8);
                                   Res = inl (0xCFC);
                                   	if (Res != 4294967295) {
                                   		int deviceID = get_dev_id (Res);
                                   		int vendorID = get_ven_id (Res);
                                   		print_vendor(vendorID);
                                   		print_device(deviceID, vendorID);
                                   			if (!is_bridge(Addr)) {
                                   				printf("Cache line size: %d Kb\n", get_cache_line_size(Addr));
                                                                              };
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
 



