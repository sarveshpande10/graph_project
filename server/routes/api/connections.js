const express = require('express')
const router = express.Router()
const connectionController = require('../../controllers/connectionController')

router.route('/')
    .post(connectionController.addConnection)
    .delete(connectionController.removeConnection)

router.route('/:userId/:friendId')
    .get(connectionController.checkConnection)

module.exports = router