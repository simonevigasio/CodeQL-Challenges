void free_last_set ( REP_SETS * sets ) {
 sets -> count -- ;
 sets -> extra ++ ;
 return ;
 }