function doGet(e) {
    return processRequest(e);
  }
  
  function doPost(e) {
    return processRequest(e);
  }
  
  function processRequest(e) {
    // Retrieve sensor data from parameters. 
    // If you’re sending a single reading, use "reading", otherwise adjust as needed.
    var reading = e.parameter.reading;  // Modify if you use "temperature" or other parameter names
  
    // Open the Google Sheet by ID and select the desired sheet (e.g., "Sheet1")
    var sheetId = "YOUR_SPREADSHEET_ID";
    var ss = SpreadsheetApp.openById(sheetId);
    var sheet = ss.getSheetByName("Sheet1") || ss.getSheets()[0]; // Use the first sheet if "Sheet1" doesn't exist
  
    // Check if this is a new sheet (no data or header row) and set headers if needed
    if (sheet.getLastRow() === 0) {
      // Set headers in the first row
      sheet.appendRow(["Timestamp", "Reading"]);
    }
  
    // Append a new row with a timestamp and the sensor reading
    var timestamp = new Date();
    var newRow = [timestamp, reading];
    sheet.appendRow(newRow);
  
    // Optionally, trigger an email alert if reading exceeds a certain threshold (e.g., 30)
    if (parseFloat(reading) > 30.0) {
      var emailAddress = "email@samba.com"; // Replace with your email address
      var subject = "Alert: Reading Threshold Exceeded";
      var body = "The sensor reading is " + reading + " at " + timestamp;
      MailApp.sendEmail(emailAddress, subject, body);
    }
  
    return ContentService.createTextOutput("Data logged successfully.");
  }