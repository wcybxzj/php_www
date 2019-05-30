<?php
//const string
$c = "hello world!";
echo $c;

$a = time()."string";
echo $a;

//copy on write
$b = $a;
echo $a;
echo $b;
$b = "hello";
echo $a;
echo $b;




