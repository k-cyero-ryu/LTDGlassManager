# LTDGlassManager
LTDGlassManager is a desktop application written in C++ designed to streamline the operations of a glass cutting and sales department. The application helps manage glass pieces, types, cutting measurements, costs, sales prices, and stock levels efficiently.

---

## 🧩 Features

- **Glass Types Management**  
  Define and categorize various types of glass (e.g., tempered, laminated, frosted, clear).

- **Cutting Operations**  
  Register pieces of glass with custom dimensions and associate them with customer orders or internal use.

- **Stock Tracking**  
  Track inventory in real-time, including available stock, used glass, and scrap or waste.

- **Pricing Management**  
  Set cost price and sales price per unit or per square meter, and automatically calculate profits.

- **Sales Records**  
  Manage customer orders and invoices, tracking what was sold, to whom, and at what price.

- **Reports**  
  Generate reports for:
  - Inventory levels
  - Sales by date/customer/glass type
  - Cutting waste and efficiency

- **Multi-user Support** (Optional)  
  Role-based access for sales agents, warehouse workers, and admins.

---

## 📷 Example Use Case

1. A client places an order for custom-sized tempered glass.
2. The system checks stock for available sheets and calculates optimal cutting.
3. After cutting, the stock is updated, including scrap tracking.
4. A sales invoice is generated automatically with correct pricing.
5. Reports show material usage, cost efficiency, and daily sales.

---

## 🖥️ Technology Stack

- **Language**: C++ (Qt or Win32/CLI, depending on implementation)
- **Database**: SQLite or MySQL (depending on configuration)
- **UI Framework**: Qt5/Qt6 (recommended) or native Windows GUI
- **Platform**: Windows Desktop

---

## 🔧 Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/yourname/GlassCutManager.git
````

2. Open the project in [Qt Creator](https://www.qt.io/download) or your preferred C++ IDE.
3. Configure the database settings in `config.h` or through the UI on first run.
4. Build the project.
5. Run the executable.

---

## 🗂️ Folder Structure

```
GlassCutManager/
├── src/                # Core source code
│   ├── main.cpp
│   ├── ui/             # UI components
│   └── models/         # Data models and business logic
├── resources/          # Icons, images, etc.
├── database/           # Initial schema and sample data
├── docs/               # Documentation
└── README.md
```

---

## 🛠️ Configuration

* Configure the database connection and currency format in `connection.cpp`.
* User roles and permissions can be edited in the database or via the admin UI.

---

## 🧪 Future Improvements

* Barcoding integration for inventory
* Glass sheet optimization algorithm
* Multi-language support
* Export reports to PDF/Excel
* Cloud sync or multi-branch support

---

## 📜 License

MIT License — see [LICENSE](LICENSE) for details.

---

## 👨‍💻 Author

Ronny Reyes
