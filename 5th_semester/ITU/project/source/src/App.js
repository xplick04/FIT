/**
 * @brief App file
 * 
 * @file App.js
 * @author Maxim Plička (xplick04, 231813)
 * @date 6.12.2022
 */

import "./App.css";
import 'react-bootstrap/dist/react-bootstrap';
import 'bootstrap/dist/css/bootstrap.min.css';

import { Route, Routes } from "react-router-dom"
import { MyNav } from "./components/inc/MyNav";
import { HomePage } from "./pages/HomePage";
import { OurAnimals } from "./pages/OurAnimals";
import { SignUp } from "./pages/SignUp";
import { AuthProvider } from "./context/AuthContext";
import { MyProfile } from "./pages/MyProfile";
import { ManageUsers } from "./pages/ManageUsers";
import { Requests } from "./pages/Requests";
import { VetRequests } from "./pages/VetRequests";
import { MyWalks } from "./pages/MyWalks";

//App with pages components and specified routing paths
function App() {
  return (
    <>
      <AuthProvider>
        <MyNav />
        <Routes>
          <Route path="/" element={<HomePage />}/>
          <Route path="/OurAnimals" element={<OurAnimals />}/>
          <Route path="/SignUp" element={<SignUp />}/>
          <Route path="/MyProfile" element={<MyProfile />}/>
          <Route path="/ManageUsers" element={<ManageUsers />}/>
          <Route path="/Requests" element={<Requests />}/>
          <Route path="/VetRequests" element={<VetRequests />}/>
          <Route path="/MyWalks" element={<MyWalks />}/>
          <Route path="*" element={<HomePage />}/>
        </Routes>
      </AuthProvider>
    </>
  );
};

export default App;
