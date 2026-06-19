const express = require('express')
const router = express.Router()
const userController = require('../../controllers/userController')

router.route('/')
    .get(userController.getAllUsers)
    .post(userController.postUser)

router.route('/:id')
    .delete(userController.deleteUser)

router.route('/:id/max-separation')
    .get(userController.getMaximumPathOfSeparation)

router.route('/:id/network')
    .get(userController.getCompleteNetwork)

module.exports = router