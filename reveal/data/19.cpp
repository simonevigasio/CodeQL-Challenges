static void pxa2xx_i2c_write ( void * opaque , hwaddr addr , uint64_t value64 , unsigned size ) {
 PXA2xxI2CState * s = ( PXA2xxI2CState * ) opaque ;
 uint32_t value = value64 ;
 int ack ;
 addr -= s -> offset ;
 switch ( addr ) {
 case ICR : s -> control = value & 0xfff7 ;
 if ( ( value & ( 1 << 3 ) ) && ( value & ( 1 << 6 ) ) ) {
 if ( value & ( 1 << 0 ) ) {
 if ( s -> data & 1 ) s -> status |= 1 << 0 ;
 else s -> status &= ~ ( 1 << 0 ) ;
 ack = ! i2c_start_transfer ( s -> bus , s -> data >> 1 , s -> data & 1 ) ;
 }
 else {
 if ( s -> status & ( 1 << 0 ) ) {
 s -> data = i2c_recv ( s -> bus ) ;
 if ( value & ( 1 << 2 ) ) i2c_nack ( s -> bus ) ;
 ack = 1 ;
 }
 else ack = ! i2c_send ( s -> bus , s -> data ) ;
 }
 if ( value & ( 1 << 1 ) ) i2c_end_transfer ( s -> bus ) ;
 if ( ack ) {
 if ( value & ( 1 << 0 ) ) s -> status |= 1 << 6 ;
 else if ( s -> status & ( 1 << 0 ) ) s -> status |= 1 << 7 ;
 else s -> status |= 1 << 6 ;
 s -> status &= ~ ( 1 << 1 ) ;
 }
 else {
 s -> status |= 1 << 6 ;
 s -> status |= 1 << 10 ;
 s -> status |= 1 << 1 ;
 }
 }
 if ( ! ( value & ( 1 << 3 ) ) && ( value & ( 1 << 6 ) ) ) if ( value & ( 1 << 4 ) ) i2c_end_transfer ( s -> bus ) ;
 pxa2xx_i2c_update ( s ) ;
 break ;
 case ISR : s -> status &= ~ ( value & 0x07f0 ) ;
 pxa2xx_i2c_update ( s ) ;
 break ;
 case ISAR : i2c_set_slave_address ( & s -> slave -> i2c , value & 0x7f ) ;
 break ;
 case IDBR : s -> data = value & 0xff ;
 break ;
 default : printf ( "%s: Bad register " REG_FMT "\n" , __FUNCTION__ , addr ) ;
 }
 }