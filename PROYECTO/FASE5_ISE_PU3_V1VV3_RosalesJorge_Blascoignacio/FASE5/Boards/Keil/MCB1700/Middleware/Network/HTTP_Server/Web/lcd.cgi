t <head><title>Temperatura</title>
#t <meta http-equiv="refresh" content="1"></meta>
t <script language=JavaScript type="text/javascript" src="xml_http.js"></script>
t <script language=JavaScript type="text/javascript">
t var formUpdate = new periodicObj("lcd.cgx", 500);
t function plotADGraph() {
t var d=0;
t }
t function periodicUpdateAd() {
t  if(document.getElementById("adChkBox").checked == true) {
t   updateMultiple(formUpdate,plotADGraph);
t   ad_elTime = setTimeout(periodicUpdateAd, formUpdate.period);
t  }
t  else
t   clearTimeout(ad_elTime);
t }
t </script></head>
t <body onload="periodicUpdateAd()">
i pg_header.inc
t <h2 align=center><br>Temperatura</h2>
t <p><font size="2">Esta pagina te permite ver el resultado de la conversión y de la temperatura</font></p>
t <form action="lcd.cgi" method="post" name="lcd">
t <input type=hidden value="lcd" name=pg><table><td>
c f 1 Medida y Temperatura: <input type=text id="medida" name=medida y temperatura size=20 maxlength=20 value="%s">
t </td><td>Cambio en el modo de adquisición:<select name="ctrl" onchange="submit();">
c f 2 <option %s>Tension</option><option %s>Frecuencia</option></select></td></tr>
t </td></table>
t <input type=button style="display: none" value="Refresh" onclick="updateMultiple(formUpdate,plotADGraph)">
t <input style="display: none" checked  type="checkbox" id="adChkBox" onload="periodicUpdateAd()">
# Here begin button definitions
t </form>
i pg_footer.inc
t </body>
. End of script must be closed with period.
