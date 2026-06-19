const mongoose = require('mongoose')
const dns = require('dns') 

// Use Google DNS to bypass corporate DNS issues 
dns.setServers(['8.8.8.8', '8.8.4.4'])
const connectDB = async () => {
    try {
        const conn = await mongoose.connect(process.env.DATABASE_URI)
    } catch(err) {
        console.error(err)
    }
}

module.exports = connectDB