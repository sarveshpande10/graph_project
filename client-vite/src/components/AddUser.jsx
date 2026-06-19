import { useState } from 'react'
import './AddUser.css'

const AddUser = ({ onAddUser }) => {
  const [formData, setFormData] = useState({
    name: '',
    hobby: '',
    educationalInstitutes: [
      {
        instituteName: '',
        startYear: '',
        endYear: '',
        currentlyStudying: false
      }
    ]
  })

  const [submitted, setSubmitted] = useState(false)

  const handleChange = (e) => {
    const { name, value } = e.target

    setFormData(prev => ({
      ...prev,
      [name]: value
    }))
  }

  const handleInstituteChange = (index, field, value) => {
    const updatedInstitutes = [...formData.educationalInstitutes]

    updatedInstitutes[index] = {
      ...updatedInstitutes[index],
      [field]: value
    }

    setFormData(prev => ({
      ...prev,
      educationalInstitutes: updatedInstitutes
    }))
  }

  const addInstitute = () => {
    setFormData(prev => ({
      ...prev,
      educationalInstitutes: [
        ...prev.educationalInstitutes,
        {
          instituteName: '',
          startYear: '',
          endYear: '',
          currentlyStudying: false
        }
      ]
    }))
  }

  const removeInstitute = (index) => {
    setFormData(prev => ({
      ...prev,
      educationalInstitutes: prev.educationalInstitutes.filter(
        (_, i) => i !== index
      )
    }))
  }

  const handleSubmit = (e) => {
    e.preventDefault()

    if (formData.name) {
      onAddUser({
        name: formData.name,
        hobby: formData.hobby,
        educationalInstitutes: formData.educationalInstitutes.map(inst => ({
          instituteName: inst.instituteName,
          startYear: Number(inst.startYear),
          endYear: inst.currentlyStudying
            ? null
            : Number(inst.endYear),
          currentlyStudying: inst.currentlyStudying
        }))
      })

      setFormData({
        name: '',
        hobby: '',
        educationalInstitutes: [
          {
            instituteName: '',
            startYear: '',
            endYear: '',
            currentlyStudying: false
          }
        ]
      })

      setSubmitted(true)
      setTimeout(() => setSubmitted(false), 3000)
    }
  }

  return (
    <div className="add-user-container">
      <div className="add-user-form">
        <h2>Add New User</h2>

        {submitted && (
          <div className="success-message">
            ✓ User added successfully!
          </div>
        )}

        <form onSubmit={handleSubmit}>
          <div className="form-group">
            <label>Full Name *</label>
            <input
              type="text"
              name="name"
              value={formData.name}
              onChange={handleChange}
              placeholder="Enter full name"
              required
            />
          </div>

          <div className="form-group">
            <label>Hobby</label>
            <input
              type="text"
              name="hobby"
              value={formData.hobby}
              onChange={handleChange}
              placeholder="Enter hobby"
            />
          </div>

          <h3>Educational Institutes</h3>

          {formData.educationalInstitutes.map((institute, index) => (
            <div
              key={index}
              style={{
                border: '1px solid #ddd',
                padding: '15px',
                marginBottom: '15px',
                borderRadius: '8px'
              }}
            >
              <div className="form-group">
                <label>Institute Name</label>
                <input
                  type="text"
                  value={institute.instituteName}
                  onChange={(e) =>
                    handleInstituteChange(
                      index,
                      'instituteName',
                      e.target.value
                    )
                  }
                />
              </div>

              <div className="form-group">
                <label>Start Year</label>
                <input
                  type="number"
                  value={institute.startYear}
                  onChange={(e) =>
                    handleInstituteChange(
                      index,
                      'startYear',
                      e.target.value
                    )
                  }
                />
              </div>

              <div className="form-group">
                <label>End Year</label>
                <input
                  type="number"
                  disabled={institute.currentlyStudying}
                  value={institute.endYear}
                  onChange={(e) =>
                    handleInstituteChange(
                      index,
                      'endYear',
                      e.target.value
                    )
                  }
                />
              </div>

              <div className="form-group">
                <label>
                  <input
                    type="checkbox"
                    checked={institute.currentlyStudying}
                    onChange={(e) =>
                      handleInstituteChange(
                        index,
                        'currentlyStudying',
                        e.target.checked
                      )
                    }
                  />
                  Currently Studying
                </label>
              </div>

              {formData.educationalInstitutes.length > 1 && (
                <button
                  type="button"
                  onClick={() => removeInstitute(index)}
                >
                  Remove Institute
                </button>
              )}
            </div>
          ))}

          <button
            type="button"
            onClick={addInstitute}
            className="submit-btn"
          >
            + Add Institute
          </button>

          <br />
          <br />

          <button type="submit" className="submit-btn">
            Add User
          </button>
        </form>
      </div>
     <div className="info-box">
        <h3>📝 Information</h3>
        <p>Add a new user to the social network. Once added, you can:</p>
        <ul>
          <li>View their profile</li>
          <li>Add them as friends</li>
          <li>See them in the network visualization</li>
          <li>Include them in network analytics</li>
        </ul>
      </div>
    </div>
  )
}

export default AddUser
