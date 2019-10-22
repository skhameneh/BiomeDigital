import React, { Component } from 'react';
import {
  StyleSheet,
  Text,
  View,
  TouchableHighlight,
  NativeEventEmitter,
  NativeModules,
  Platform,
  PermissionsAndroid,
  ScrollView,
  AppState,
  Dimensions,
} from 'react-native';
import BleManager from 'react-native-ble-manager';
import ListView from 'deprecated-react-native-listview';

const {
  ACCESS_COARSE_LOCATION,
} = PermissionsAndroid.PERMISSIONS;

const window = Dimensions.get('window');
const ds = new ListView.DataSource({rowHasChanged: (r1, r2) => r1 !== r2});

const service = 'bb45a210-56c6-4d35-8840-35e57dc20994';
const networkCharacteristic = 'a38bfd96-f6ff-4b5b-858b-80f2abdd1859';
const CHARACTERISTIC_UUID_SSID = '6c754c96-8410-45de-bb04-e2aee7851393';
const CHARACTERISTIC_UUID_AUTH = '714fedf0-e6f7-4d7b-b7a4-197b1ae04840';

const BleManagerModule = NativeModules.BleManager;
const bleManagerEmitter = new NativeEventEmitter(BleManagerModule);

let selectedPeripheralId = null;

const bytesToString = (buffer) => String.fromCharCode.apply(null, new Uint8Array(buffer));
const stringToByteArray = (string) => string.split('').map((x) => x.charCodeAt(0));
const asyncTimeout = (ms) => (new Promise(resolve => setTimeout(resolve, ms)));

export default class App extends Component {
  state = {
    scanning:false,
    peripherals: new Map(),
    appState: '',
    networks: [],
  }

  handleDiscoverPeripheral = this.handleDiscoverPeripheral.bind(this);
  handleStopScan = this.handleStopScan.bind(this);
  handleUpdateValueForCharacteristic = this.handleUpdateValueForCharacteristic.bind(this);
  handleDisconnectedPeripheral = this.handleDisconnectedPeripheral.bind(this);
  handleAppStateChange = this.handleAppStateChange.bind(this);

  async checkAndRequestPermissions() {
    if (Platform.OS === 'android' && Platform.Version >= 23) {
      const permissionsCheckResult = await PermissionsAndroid.check(ACCESS_COARSE_LOCATION);
      if (permissionsCheckResult) {
        return true;
      }

      return this.requestPermissions();
    }
  }

  async requestPermissions() {
    const permissionsRequestResult = PermissionsAndroid.requestPermission(ACCESS_COARSE_LOCATION);
      if (permissionsRequestResult) {
        return true;
      } else {
        console.log('requestPermission failed.');

        return false;
      }
  }

  componentDidMount() {
    AppState.addEventListener('change', this.handleAppStateChange);

    BleManager.start({ showAlert: false });

    this.handlerDiscover = bleManagerEmitter.addListener('BleManagerDiscoverPeripheral', this.handleDiscoverPeripheral );
    this.handlerStop = bleManagerEmitter.addListener('BleManagerStopScan', this.handleStopScan );
    this.handlerDisconnect = bleManagerEmitter.addListener('BleManagerDisconnectPeripheral', this.handleDisconnectedPeripheral );
    this.handlerUpdate = bleManagerEmitter.addListener('BleManagerDidUpdateValueForCharacteristic', this.handleUpdateValueForCharacteristic );

    this.checkAndRequestPermissions().then(() => {
      this.startScan();
    });
  }

  handleAppStateChange(nextAppState) {
    if (this.state.appState.match(/inactive|background/) && nextAppState === 'active') {
      console.log('App has come to the foreground!')
      BleManager.getConnectedPeripherals([]).then((peripheralsArray) => {
        console.log('Connected peripherals: ' + peripheralsArray.length);
      });
    }
    this.setState({appState: nextAppState});
  }

  componentWillUnmount() {
    this.handlerDiscover.remove();
    this.handlerStop.remove();
    this.handlerDisconnect.remove();
    this.handlerUpdate.remove();
  }

  handleDisconnectedPeripheral(data) {
    let peripherals = this.state.peripherals;
    let peripheral = peripherals.get(data.peripheral);
    if (peripheral) {
      peripheral.connected = false;
      peripherals.set(peripheral.id, peripheral);
      this.setState({peripherals});
    }
    console.log('Disconnected from ' + data.peripheral);
  }

  async handleUpdateValueForCharacteristic(data) {
    console.log('Received data from ' + data.peripheral + ' characteristic ' + data.characteristic);

    if(data.characteristic === networkCharacteristic) {
      const rawNetworksArray = bytesToString(
        await BleManager.read(data.peripheral, data.service, data.characteristic)
      ).split(/\n/g);

      const networks = [];

      for(let i = 0; i < rawNetworksArray.length; i += 4) {
        const [
          ssid,
          channel,
          rssi,
          encryptionType,
        ] = rawNetworksArray.slice(i, i + 4);

        if(ssid) {
          networks.push({
            ssid,
            channel,
            rssi,
            encryptionType,
          });
        }
      }

      console.log({ networks });

      this.setState({ networks });
    }
  }

  handleStopScan() {
    console.log('Scan is stopped');
    this.setState({ scanning: false });
  }

  async startScan() {
    if(!this.state.scanning) {
      this.setState({peripherals: new Map()});

      const startedScan = await BleManager.scan([], 10, true);
      console.log({ startedScan });
    }
  }

  async retrieveConnected() {
    const results = await BleManager.getConnectedPeripherals([]);

    if (results.length == 0) {
      console.log('No connected peripherals')
    }

    console.log(results);

    const {
      peripherals
    } = this.state;

    for(let i = 0; i < results.length; i++) {
      let peripheral = results[i];
      peripheral.connected = true;
      peripherals.set(peripheral.id, peripheral);
      this.setState({ peripherals });
    }
  }

  handleDiscoverPeripheral(peripheral){
    var peripherals = this.state.peripherals;
    if (!peripherals.has(peripheral.id)){
      console.log('Got ble peripheral', peripheral);
      peripherals.set(peripheral.id, peripheral);
      this.setState({ peripherals })
    }
  }

  async connectPeripheral(selectedPerpheral) {

    if(selectedPerpheral) {

      if(selectedPerpheral.connected) {
        BleManager.disconnect(selectedPerpheral.id);
        return;
      }

      const connectResult = await BleManager.connect(selectedPerpheral.id);
      
      BleManager.requestMTU(selectedPerpheral.id, 247).then((mtu) => {
        console.log('MTU size changed to ' + mtu + ' bytes');
      }).catch((error) => {
        console.log(error);
      });
      
      const {
        peripherals
      } = this.state;

      const peripheral = peripherals.get(selectedPerpheral.id);
      selectedPeripheralId = selectedPerpheral.id;

      if(peripheral) {
        peripheral.connected = true;
        peripherals.set(peripheral.id, peripheral);
        this.setState({ peripherals });
      }

      console.log('Connected to ' + peripheral.id);

      const peripheralInfo = await BleManager.retrieveServices(peripheral.id);

      console.log(peripheralInfo);

      //const cResponse = await BleManager.read(peripheral.id, service, networkCharacteristic);
      BleManager.startNotification(peripheral.id, service, networkCharacteristic);
    }
  }

  async selectNetwork(network, tries = 5, retryWaitMs = 500) {
    console.log('write', service, CHARACTERISTIC_UUID_SSID, network.ssid);

    const writeResult = await BleManager.write(
      selectedPeripheralId,
      service,
      CHARACTERISTIC_UUID_SSID,
      stringToByteArray(network.ssid)
    ).catch(() => false);

    if(writeResult !== false) {
      return true;
    }

    if(tries) {
      await asyncTimeout(retryWaitMs);

      return this.selectNetwork(network, --tries);
    }

    return false;
  }

  async setNetworkAuth(auth, tries = 5, retryWaitMs = 500) {
    console.log('write', service, CHARACTERISTIC_UUID_AUTH);

    const writeResult = await BleManager.write(
      selectedPeripheralId,
      service,
      CHARACTERISTIC_UUID_AUTH,
      stringToByteArray(auth)
    ).catch(() => false);

    if(writeResult) {
      return true;
    }

    if(tries) {
      await asyncTimeout(retryWaitMs);

      return this.setNetworkAuth(network, --tries);
    }

    return false;
  }

  render() {
    const list = Array.from(this.state.peripherals.values());
    const {
      networks,
    } = this.state;
    const dataSource = ds.cloneWithRows(list);


    return (
      <View style={styles.container}>
        <TouchableHighlight style={{marginTop: 40,margin: 20, padding:20, backgroundColor:'#ccc'}} onPress={() => this.startScan() }>
          <Text>Scan Bluetooth ({this.state.scanning ? 'on' : 'off'})</Text>
        </TouchableHighlight>
        <ScrollView style={styles.scroll}>
          {(networks.length == 0) &&
            <View style={{flex:1, margin: 20}}>
              <Text style={{textAlign: 'center'}}>No networks</Text>
            </View>
          }
          <ListView
            enableEmptySections={true}
            dataSource={ds.cloneWithRows(networks)}
            renderRow={(network) => {
              return (
                <TouchableHighlight onPress={async () => {
                  if(await this.selectNetwork(network)) {
                    const auth = prompt('Auth');

                    if(auth) {
                      this.setNetworkAuth(auth);
                    }
                  }
                }}>
                  <View style={[styles.row, { backgroundColor: 'white' }]}>
                    <Text style={{fontSize: 12, textAlign: 'center', color: '#333333', padding: 10}}>{network.ssid}</Text>
                    <Text style={{fontSize: 8, textAlign: 'center', color: '#333333', padding: 10}}>{network.rssi}</Text>
                  </View>
                </TouchableHighlight>
              );
            }}
          />
        </ScrollView>
        <ScrollView style={styles.scroll}>
          {(list.length == 0) &&
            <View style={{flex:1, margin: 20}}>
              <Text style={{textAlign: 'center'}}>No peripherals</Text>
            </View>
          }
          <ListView
            enableEmptySections={true}
            dataSource={dataSource}
            renderRow={(item) => {
              const color = item.connected ? 'green' : '#fff';
              return (
                <TouchableHighlight onPress={() => this.connectPeripheral(item) }>
                  <View style={[styles.row, {backgroundColor: color}]}>
                    <Text style={{fontSize: 12, textAlign: 'center', color: '#333333', padding: 10}}>{item.name}</Text>
                    <Text style={{fontSize: 8, textAlign: 'center', color: '#333333', padding: 10}}>{item.id}</Text>
                  </View>
                </TouchableHighlight>
              );
            }}
          />
        </ScrollView>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#FFF',
    width: window.width,
    height: window.height
  },
  scroll: {
    flex: 1,
    backgroundColor: '#f0f0f0',
    margin: 10,
  },
  row: {
    margin: 10
  },
});