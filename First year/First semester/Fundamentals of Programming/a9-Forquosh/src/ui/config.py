import configparser

from src.repository.book_repository import BookBinaryFileRepository, BookTextFileRepository, BookMemoryRepository
from src.repository.client_repository import ClientBinaryFileRepository, ClientTextFileRepository, ClientMemoryRepository
from src.repository.rental_repository import RentalMemoryRepository, RentalTextFileRepository, RentalBinaryFileRepository


def settings_book_repository():
    config = configparser.ConfigParser()
    config.read('settings.properties')
    repository_type = config['SETTINGS']['book_repository_type']

    if repository_type == 'BinaryFile':
        return BookBinaryFileRepository()
    if repository_type == 'TextFile':
        return BookTextFileRepository()
    if repository_type == 'Memory':
        return BookMemoryRepository()


def settings_client_repository():
    config = configparser.ConfigParser()
    config.read('settings.properties')
    repository_type = config['SETTINGS']['client_repository_type']

    if repository_type == 'BinaryFile':
        return ClientBinaryFileRepository()
    if repository_type == 'TextFile':
        return ClientTextFileRepository()
    if repository_type == 'Memory':
        return ClientMemoryRepository()


def settings_rental_repository():
    config = configparser.ConfigParser()
    config.read('settings.properties')
    repository_type = config['SETTINGS']['rental_repository_type']

    if repository_type == 'BinaryFile':
        return RentalBinaryFileRepository()
    if repository_type == 'TextFile':
        return RentalTextFileRepository()
    if repository_type == 'Memory':
        return RentalMemoryRepository()


current_book_repository = settings_book_repository()
current_client_repository = settings_client_repository()
current_rental_repository = settings_rental_repository()
