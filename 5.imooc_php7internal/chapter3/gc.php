<?php
for($i = 0; $i <= 10002; $i++) {            	
	$a[$i] = $i."_string";   
	$b[] = $a[$i];   
	unset($a[$i]);
}
unset($b[0]);
echo $b[0];

