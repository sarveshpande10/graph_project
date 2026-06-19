import { useState } from 'react'
import './SearchUsers.css'

const SearchUsers = ({ onSearch, results, onUserSelect, onSelectTab }) => {
  const [query, setQuery] = useState('')

  const handleInputChange = (e) => {
    const value = e.target.value
    setQuery(value)
    onSearch(value)
  }

  const handleSelectUser = (user) => {
    onUserSelect(user)
    onSelectTab()
  }

  return (
    <div className="search-container">
      <div className="search-box">
        <input
          type="text"
          placeholder="🔍 Search by name or hobby..."
          value={query}
          onChange={handleInputChange}
          className="search-input"
        />
      </div>

      <div className="search-results">
        {query.trim() === '' ? (
          <div className="empty-state">
            <p>Enter a name or hobby to search for users</p>
          </div>
        ) : results.length === 0 ? (
          <div className="empty-state">
            <p>No users found matching "{query}"</p>
          </div>
        ) : (
          <div className="results-list">
            <h3>Found {results.length} user{results.length !== 1 ? 's' : ''}</h3>
            {results.map(user => (
              <div key={user.id} className="result-item">
                <div className="result-avatar">{user.name.charAt(0)}</div>
                <div className="result-info">
                  <h4>{user.name}</h4>
                  <p>Hobby: {user.hobby}</p>
                  <small>Friends: {user.friends.length}</small>
                </div>
                <button
                  className="view-btn"
                  onClick={() => handleSelectUser(user)}
                >
                  View Profile
                </button>
              </div>
            ))}
          </div>
        )}
      </div>
    </div>
  )
}

export default SearchUsers
