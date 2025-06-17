<?php
session_start();
require_once '../config/db.php';

if (empty($_SESSION['user_id'])) {
    // User is not logged in
    header("Location: ../login.php");
    exit();
}

try {
    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        $propertyId = $_POST['id'] ?? null;
        
        if ($propertyId) {
            // First, verify the user owns this property
            $stmt = $pdo->prepare("SELECT COUNT(*) FROM UserToProperties WHERE idUser = ? AND idProperty = ?");
            $stmt->execute([$_SESSION['user_id'], $propertyId]);
            $userOwnsProperty = $stmt->fetchColumn();
            
            if ($userOwnsProperty == 0) {
                throw new Exception("Error: Property not found or you do not have permission to delete it.");
            }
            
            // Delete the user-property relationship
            $stmt = $pdo->prepare("DELETE FROM UserToProperties WHERE idUser = ? AND idProperty = ?");
            $stmt->execute([$_SESSION['user_id'], $propertyId]);
            
            // Check if any other users are still associated with this property
            $stmt = $pdo->prepare("SELECT COUNT(*) FROM UserToProperties WHERE idProperty = ?");
            $stmt->execute([$propertyId]);
            $propertyCount = $stmt->fetchColumn();
            
            if ($propertyCount == 0) {
                // If no users are left associated with the property, delete the property itself
                $stmt = $pdo->prepare("DELETE FROM Property WHERE id = ?");
                $stmt->execute([$propertyId]);
            }
            // If $propertyCount > 0, other users still own this property, so we don't delete it
            // This is normal and not an error condition
            
        } else {
            throw new Exception("Property ID is required for deletion.");
        }
        
        header("Location: ../index.php");
        exit();
    }
} catch (Exception $e) {
    $errorMsg = urlencode($e->getMessage());
    header("Location: ../index.php?error=$errorMsg");
    exit();
}
?>