--TEST--
asyscon.os-machine setting
--INI--
asyscon.os-machine = OS_MACHINE
--FILE--
<?php
echo "Machine: " . OS_MACHINE . "\n";
?>
--EXPECTREGEX--
Machine: .*
