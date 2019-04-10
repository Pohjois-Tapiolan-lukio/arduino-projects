# IFTTT
1. Luo IFTTT-tili.
2. Luo uusi Applet (yläkulmassa oleva profiilikuvake > New Applet).
3. Valitse _this_.
4. Kirjoita hakuun _Webhooks_ ja valitse ikoni jossa lukee _Webhooks_.
5. Valitse _Receive a web request_.
6. Anna tälle webhookille nimi, ja pistä tämä muistiin.
7. Valitse _Create trigger_.
8. Valitse _that_.
9. Kirjoita hakuun _Google Sheets_ ja valitse ikoni jossa lukee _Google Sheets_.
10. Valitse _Add row to spreadsheet_.
11. Valitse _Create action_.
12. Siirry My Applets > Services > Webhooks > Settings
    (tai [klikkaa tästä](https://ifttt.com/services/maker_webhooks/settings))
    ja kopioi URL-kohdan viimeinen osa
    (`https://maker.ifttt.com/use/`:n jälkeen oleva sotku).
13. Lähetä POST-pyyntö seuraavanlaiseen URL:iin:
    `https://maker.ifttt.com/trigger/<event>/with/key/<avain>`
    missä `<event>` on nimi jonka annoit kohdassa 6, ja `<avain>` on
    henkilökohtainen avaimesi jonka kopioit kohdassa 12.
    * Tämä tapahtuu esimerkiksi Pythonilla seuraavalla koodilla:
      ```python
      import requests
      requests.post("https://maker.ifttt.com/trigger/<event>/with/key/<avain>")
      ```
