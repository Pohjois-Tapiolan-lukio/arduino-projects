function getAllData() {
    var firebaseUrl = "FIREBASE PROJEKTIN OSOITE/Mittaukset";
    var secret ="FIRE BASE PROJEKTIN AVAIN";
    var base = FirebaseApp.getDatabaseByUrl(firebaseUrl, secret);
    var data = base.getData();

    var ss = SpreadsheetApp.openById("GOOGLE SHEETS TIEDOSTON ID");
    var sheet = ss.getSheets()[0];

    var num = 2;
    range = sheet.getRange("A" + num + ":F" + num + "");
    for(var i in data) {
        var values = [
            [ data[i].Aika, data[i].Kosteus, data[i].Lampotila, data[i].LampotilaBMP, data[i].Korkeus, data[i].Paine]
        ];
        range.setValues(values);
        num += 1;
        range = sheet.getRange("A" + num + ":F" + num + "");
    }
}
