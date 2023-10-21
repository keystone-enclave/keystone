SECTIONS {                                                                                                           
    .keystone : {                                                                                                    
        *(.keystone*)                                                                                                
    }                                                                                                                
                                                                                                                     
    .bss : {                                                                                                         
        *(.bss*)                                                                                                     
        LONG(0)                                                                                                      
    }                                                                                                                
                                                                                                                     
    .sbss : {                                                                                                        
        *(.sbss*)                                                                                                    
        LONG(0)                                                                                                      
    }                                                                                                                
}         

INSERT BEFORE .text;