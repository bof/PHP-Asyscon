--TEST--
asyscon.host-fqdn setting
--INI--
asyscon.host-fqdn = SERVER_FQDN
--FILE--
<?php
echo "Server FQDN: " . SERVER_FQDN . "\n";
?>
--EXPECTREGEX--
Server FQDN: [a-zA-Z_][a-zA-Z0-9_]*\..*
