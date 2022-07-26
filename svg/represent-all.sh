#!/bin/bash
echo '<html><head></head><body><table width="100%">'
for f in *.svg; do 
    echo '<tr style="height:300px"><td width="50%" style="text-align: center; vertical-align: top;">'
    cat $f
    echo '</td><td width="50%" style="text-align: center; vertical-align: top;">'
    ../bin/represent $f
    echo '</td></tr>'
done
echo '</table></body></html>'