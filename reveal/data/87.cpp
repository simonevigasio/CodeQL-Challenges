static uint64_t pxa2xx_i2c_read ( void * opaque , hwaddr addr , unsigned size ) {
 PXA2xxI2CState * s = ( PXA2xxI2CState * ) opaque ;
 addr -= s -> offset ;
 switch ( addr ) {
 case ICR : return s -> control ;
 case ISR : return s -> status | ( i2c_bus_busy ( s -> bus ) << 2 ) ;
 case ISAR : return s -> slave -> i2c . address ;
 case IDBR : return s -> data ;
 case IBMR : if ( s -> status & ( 1 << 2 ) ) s -> ibmr ^= 3 ;
 else s -> ibmr = 0 ;
 return s -> ibmr ;
 default : printf ( "%s: Bad register " REG_FMT "\n" , __FUNCTION__ , addr ) ;
 break ;
 }
 return 0 ;
 }