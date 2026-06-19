const CheckConnection = ({ 
    sourceUser, 
    setSourceUser, 
    users, 
    targetUser, 
    setTargetUser, 
    checkConnection, 
    connectionResult 
}) => {
    return (
        <div className="tool-card">
        <h3>Check Connection</h3>

        <select
            value={sourceUser || ""}
            onChange={(e) => setSourceUser(e.target.value)}
        >
            <option value="">Select User 1</option>
            {users.map(user => (
            <option key={user._id} value={user._id}>
                {user.name}
            </option>
            ))}
        </select>

        <select
            value={targetUser || ""}
            onChange={(e) => setTargetUser(e.target.value)}
        >
            <option value="">Select User 2</option>
            {users.map(user => (
            <option key={user._id} value={user._id}>
                {user.name}
            </option>
            ))}
        </select>

        <button onClick={checkConnection}>
            Check
        </button>

        {connectionResult && 
            Array.isArray(connectionResult) ? 
            <>
                <p>Connected Via:</p>
                {connectionResult.map((name, index) => 
                    <p key={name}>{name} {index != connectionResult.length-1 ? "=>" : ""}</p>
                )} 
            </> 
            : <p>{connectionResult}</p> 
        }
        </div>
    )
}

export default CheckConnection
