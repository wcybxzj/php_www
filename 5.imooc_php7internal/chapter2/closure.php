<?php
class Test {    
      private $num = 1;
}

$f = function() {    
      return $this->num + 1;
};

echo $f->call(new Test)."\n";

