# Social Network Graph - Express Backend

This is an Express.js backend server that implements the same social network graph logic from the C++ application.

## Installation

```bash
npm install
```

## Running the Server

**Development mode (with auto-reload):**
```bash
npm run dev
```

**Production mode:**
```bash
npm start
```

The server runs on `http://localhost:5000`

## API Endpoints

### Users Management

#### Create a new user
```
POST /api/users
Content-Type: application/json

{
  "name": "John Doe",
  "hobby": "Reading",
  "educationalInstitutes": [
    {
      "instituteName": "MIT",
      "startYear": 2020,
      "endYear": 2024,
      "currentlyStudying": true
    }
  ]
}
```

#### Get all users
```
GET /api/users
```

#### Get user by ID
```
GET /api/users/:id
```

#### Delete user
```
DELETE /api/users/:id
```

### Connections/Friendships

#### Connect two users
```
POST /api/connections
Content-Type: application/json

{
  "userId": 1,
  "friendId": 2
}
```

#### Disconnect two users
```
DELETE /api/connections
Content-Type: application/json

{
  "userId": 1,
  "friendId": 2
}
```

#### Get contact list
```
GET /api/users/:id/contacts
```

### Network Analysis

#### Get connection path between two users
```
GET /api/users/:userId/connection/:friendId
```

Returns the path and degree of separation.

#### Get current school/college mates
```
GET /api/users/:id/current-mates
```

#### Get past school/college mates
```
GET /api/users/:id/past-mates
```

#### Get complete network
```
GET /api/users/:id/network
```

Shows all connections reachable from a user.

#### Get maximum path of separation
```
GET /api/users/:id/max-separation
```

Returns the farthest user from a given user in the network.

### Search

#### Search users by hobby
```
GET /api/search/hobby/:hobby
```

### Statistics

#### Get graph statistics
```
GET /api/stats
```

Returns total users, nodes, and dead nodes.

### Health Check

```
GET /api/health
```

## Example Usage

```bash
# Create first user
curl -X POST http://localhost:5000/api/users \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Alice",
    "hobby": "Coding",
    "educationalInstitutes": [{"instituteName": "MIT", "startYear": 2020, "endYear": 2024, "currentlyStudying": true}]
  }'

# Create second user
curl -X POST http://localhost:5000/api/users \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Bob",
    "hobby": "Gaming",
    "educationalInstitutes": [{"instituteName": "MIT", "startYear": 2020, "endYear": 2024, "currentlyStudying": true}]
  }'

# Connect them
curl -X POST http://localhost:5000/api/connections \
  -H "Content-Type: application/json" \
  -d '{"userId": 1, "friendId": 2}'

# Get contacts
curl http://localhost:5000/api/users/1/contacts
```

## Features

- **User Management**: Add, retrieve, and delete users
- **Friendship Connections**: Create and remove connections between users
- **Institute Matching**: Find current and past classmates
- **Path Finding**: Calculate shortest path between users (degree of separation)
- **Network Analysis**: Analyze complete network and find maximum separation
- **Search**: Find users by hobby
- **In-memory storage**: All data stored in memory (can be extended with database)

## Technologies Used

- Express.js
- Node.js
- CORS
- Body-parser
