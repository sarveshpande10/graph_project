require('dotenv').config()
const express = require("express");
const cors = require("cors");
const bodyParser = require("body-parser");
const mongoose = require('mongoose')
const connectDB = require('./config/dbConn')

const app = express();
const PORT = process.env.PORT || 3500;

connectDB()

// Middleware
app.use(cors({
    origin: 'http://localhost:5173',
    credentials: true
}))
app.use(bodyParser.json());

const logger = (req, res, next) => {
    console.log(req.url, req.method);
    next()
}

app.use(logger)


// Routes
app.use("/api/users", require('./routes/api/users'));
app.use("/api/connections", require('./routes/api/connections'))


// 4. Delete user

// 5. Connect two users (add friendship)

// 6. Disconnect two users (remove friendship)

// 7. Get contact list for a user
//Moved to frontend

// 8. Get current school/college mates
//Moved to frontend

// 9. Get past school/college mates
//Moved to frontend

// 10. Check if two users are connected and get path

// 11. Get complete network for a user

// 12. Get maximum path of separation

// 13. Search users by hobby
//Moved to frontend

// Start server
mongoose.connection.once('open', () => {
    console.log('Connected to MongoDB');
    app.listen(PORT, () => console.log(`Server running on port ${PORT}`))
})
