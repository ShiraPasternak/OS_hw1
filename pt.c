#include <stdio.h>
#include "os.h"
#include <inttypes.h>

uint64_t page_table_query(uint64_t pt, uint64_t vpn);
void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn);
    
int bitValidOn (uint64_t bits);
uint64_t getSymbol (uint64_t vpn, int level);


void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn){
	uint64_t *PTEs;
    uint64_t pageTablePpn, pte, symbol;
	pageTablePpn = pt;
	
	if (ppn == NO_MAPPING) {
		for (int i = 5; i >= 1; i--) {
			symbol = getSymbol(vpn, i);
    		PTEs = phys_to_virt(pageTablePpn << 12);
    		pte = PTEs[symbol];
    		if (!bitValidOn(pte)){ //if theres no mapping, stop run
        		return;
    		}
    		pageTablePpn = pte >> 12;
    	}
    	PTEs[symbol] = pte & ~1; //turning off the valid bit
    }
    else {
		for (int i = 5; i >= 2; i--) {
			symbol = getSymbol(vpn, i);
    		PTEs = phys_to_virt(pageTablePpn << 12);
    		pte = PTEs[symbol];
    		if (bitValidOn(pte)){
    			pageTablePpn = pte >> 12;
    		}
    		else {
    			pageTablePpn = alloc_page_frame();
    			PTEs[symbol] = pageTablePpn << 12;
    			PTEs[symbol] = PTEs[symbol] | 1; //turning on the valid bit
    		}
    	
    	}
    	//assigning ppn
    	symbol = getSymbol(vpn, 1);
    	PTEs = phys_to_virt(pageTablePpn << 12);
    	PTEs[symbol] = ppn << 1;
    	PTEs[symbol] = PTEs[symbol] | 1; //turning on the valid bit
    }
}

uint64_t page_table_query(uint64_t pt, uint64_t vpn){
    uint64_t *PTEs;
    uint64_t pte, pageFrame, symbol, ppn;
    pageFrame = pt;
    for (int i = 5; i >= 1; i--){
        symbol = getSymbol(vpn, i);
        PTEs = phys_to_virt(pageFrame << 12);
        pte = PTEs[symbol];
        if (!bitValidOn(pte)){
            return NO_MAPPING;
        }
        pageFrame = pte >> 12;
    }
    ppn = pte >> 1; //level 1 pte has the ppn without valid bit
    return ppn; 
}
    
    
int bitValidOn (uint64_t bits) {
    if ((bits & 1) == 1) {
        return 1;
    }
    else {
        return 0;
    }
}

uint64_t getSymbol (uint64_t vpn, int level) {
    uint64_t symbol, mask = 511;
    symbol = vpn >> 9 * (level - 1);
    return symbol & mask;
}
