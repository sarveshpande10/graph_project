const mongoose = require('mongoose')
const Schema = mongoose.Schema
// const instituteSchema = require('./Institute')

const instituteSchema = new Schema({
  instituteName: {
    type: String,
  },
  startYear: {
    type: Number,
  },
  endYear: {
    type: Number
  },
  currentlyStudying: {
    type: Boolean,
    default: false
  }
})

const userSchema = new mongoose.Schema({
  name: {
    type: String,
    required: true 
  }, 
  hobby: {
    type: String,
    default: ""
  }, 
  educationalInstitutes: [instituteSchema],
  friends: [{
    type: mongoose.Schema.Types.ObjectId,
    ref: "User"
  }]
});

module.exports = mongoose.model('User', userSchema)