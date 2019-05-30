<?php
$arr = [1, 2, 3];
list($a, $b, $c) = $arr;
var_dump($a, $b, $c);


$arr = [1, 2, 3];
[$a, $b, $c] = $arr;
var_dump($a, $b, $c);

