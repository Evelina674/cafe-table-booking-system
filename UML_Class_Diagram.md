# UML архітектура системи бронювання столиків
```mermaid
classDiagram
direction TB

class User{
  +id : int
  +username : QString
  +passwordHash : QString
  +role : UserRole
  +isValid() : bool
  +isAdmin() : bool
}

class Table{
  +id : int
  +number : int
  +capacity : int
  +description : QString
  +isValid() : bool
}

class Reservation{
  +id : int
  +userId : int
  +tableId : int
  +date : QDate
  +timeStart : QTime
  +timeEnd : QTime
  +guestCount : int
  +status : ReservationStatus
  +createdAt : QDateTime
  +isValid() : bool
  +isActive() : bool
  +isPast() : bool
}

class UserRole{
<<enumeration>>
Admin
User
}

class ReservationStatus{
<<enumeration>>
Active
Cancelled
}

User "1" --> "0..*" Reservation : userId
Table "1" --> "0..*" Reservation : tableId

User ..> UserRole
Reservation ..> ReservationStatus
```
