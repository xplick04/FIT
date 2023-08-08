from rest_framework.decorators import api_view, permission_classes
from rest_framework.response import Response
from rest_framework_simplejwt.views import TokenObtainPairView
from rest_framework.permissions import IsAuthenticated
from rest_framework import permissions

from .models import UserModel
from .serializers import UserSerializer, MyTokenObtainPairSerializer
from .utils import hasRole, getUsername

class MyTokenObtainPairView(TokenObtainPairView):
	serializer_class = MyTokenObtainPairSerializer

@api_view(['POST'])
def registerUser(request):
	serializer = UserSerializer(data=request.data)
	if serializer.is_valid():
		if not hasRole(request, 'Administrator'):
			if (request.data.get('verified') is not None) or (request.data.get('type') is not None):
				return Response('You do not have permission.')
		user = serializer.save()
		user.set_password(serializer.data.get('password'))
		user.save()
		response = 'User is registered.'
	else:
		response = 'User is not registered.'
	return Response(response)

@api_view(['POST', 'GET'])
@permission_classes([IsAuthenticated])
def detailUser(request, pk):
	username = getUsername(pk)
	if request.method == 'GET':
		if (not hasRole(request, 'Administrator')) and (not hasRole(request, 'Caretaker')):
			if username != request.user.username:
				return Response('You do not have permission.')
		user = UserModel.objects.get(id=pk)
		serializer = UserSerializer(user, many=False)
		return Response(serializer.data)

	data = request.data
	try:
		first_name = data['first_name']
		last_name = data['last_name']
		date_of_birth = data['date_of_birth']
	except:
		return Response('User is not updated.')

	if not hasRole(request, 'Administrator'):
		if (username != request.user.username):
			return Response('You do not have permission.')
	
	user = UserModel.objects.get(id=pk)
	user.first_name = first_name
	user.last_name = last_name
	user.date_of_birth = date_of_birth
	user.save()
	return Response('User is updated.')

@api_view(['POST'])
@permission_classes([IsAuthenticated])
def uploadPhoto(request, pk):
	username = getUsername(pk)
	data = request.data
	try:
		photo = data['photo']
	except:
		return Response('User is not updated.')

	if not hasRole(request, 'Administrator'):
		if (username != request.user.username):
			return Response('You do not have permission.')
	
	user = UserModel.objects.get(id=pk)
	user.photo = photo
	user.save()
	return Response('User is updated.')

@api_view(['GET'])
@permission_classes([IsAuthenticated])
def listUser(request):
	if not hasRole(request, 'Administrator'):
		return Response('You do not have permission.')
	users = UserModel.objects.all()
	serializer = UserSerializer(users, many=True)
	return Response(serializer.data)

@api_view(['DELETE'])
@permission_classes([IsAuthenticated])
def deleteUser(request, pk):
	username = getUsername(pk)
	if not hasRole(request, 'Administrator'):
		if username != request.user.username:
			return Response('You do not have permission.')
	try:
		user = UserModel.objects.get(id=pk)
		user.delete()
		response = 'User is deleted.'
	except:
		response = 'User is not deleted.'
	return Response(response)

@api_view(['POST'])
@permission_classes([IsAuthenticated])
def verifyUser(request, pk):
	if not hasRole(request, 'Caretaker'):
		return Response('You do not have permission.')

	user = UserModel.objects.get(id=pk)
	user.verified = True
	user.save()

	return Response('User is verified.')

@api_view(['POST'])
@permission_classes([IsAuthenticated])
def typeUser(request, pk):
	if not hasRole(request, 'Administrator'):
		return Response('You do not have permission.')
	data = request.data
	try:
		type = data['type']
	except:
		return Response('User is not updated.')
	
	user = UserModel.objects.get(id=pk)
	user.type = type
	user.save()
	return Response('User is updated.')

@api_view(['GET'])
@permission_classes([IsAuthenticated])
def unverifiedUsers(request):
	if not hasRole(request, 'Caretaker'):
		return Response('You do not have permission.')
	users = UserModel.objects.filter(verified=False)
	serializer = UserSerializer(users, many=True)
	return Response(serializer.data)
