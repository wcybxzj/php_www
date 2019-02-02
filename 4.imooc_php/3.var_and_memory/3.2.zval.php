<?php
//这里echo 是为了方便gdb设置断点进行debug
$a=2;
echo $a;
$b=1.1;
echo $b;
$c=null;
echo $c;
$d=true;
echo $d;
$e=false;
echo $e;
$f="ybxisme";
echo $f;
$g=[1,2,3];
echo $g;
$h=new stdclass($h);
echo $h;//FATAL ERROR
